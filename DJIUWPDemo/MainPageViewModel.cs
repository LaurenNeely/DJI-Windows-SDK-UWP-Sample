using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using System.Windows.Input;
using Windows.Graphics.Imaging;
using Windows.Media;
using Windows.Storage.Streams;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media.Imaging;

namespace DJIDemo
{
    public class MainPageViewModel : INotifyPropertyChanged
    {
        private CoreDispatcher Dispatcher;
        private DJIClient djiClient;

        private InkShapes.InkShapesModel mlModel = null;
        private Task runProcessTask = null;

 
        public MainPageViewModel(CoreDispatcher dispatcher, DJIClient djiClient)
        {
            this.Dispatcher = dispatcher;
            this.djiClient = djiClient;
            djiClient.ConnectedChanged += DjiClient_ConnectedChanged;
            djiClient.FlyingChanged += DjiClient_FlyingChanged;
            djiClient.AltitudeChanged += DjiClient_AltitudeChanged;
            djiClient.AttitudeChanged += DjiClient_AttitudeChanged;
            djiClient.VelocityChanged += DjiClient_VelocityChanged;
            djiClient.FrameArived += DjiClient_FrameArived;
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void RaisepropertyChanged([CallerMemberName] string propertyName = null)
        {
            if (Dispatcher.HasThreadAccess)
            {
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            }
            else
            {
                var ignore = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
                });
            }
        }

        private bool isConnected = false;
        public bool IsConnected
        {
            get => isConnected;
            set
            {
                isConnected = value;
                RaisepropertyChanged();
                RaisepropertyChanged(nameof(ConnectedStatus));
                RaisepropertyChanged(nameof(ControlsVisible));
            }
        }

        private bool isFlying = false;
        public bool IsFlying
        {
            get => isFlying;
            set
            {
                isFlying = value;
                RaisepropertyChanged();
            }
        }

        public string ConnectedStatus
        {
            get
            {
                if (IsConnected)
                {
                    return "Connected";
                }
                else
                {
                    return "Connecting...";
                }
            }
        }


        private int gimpbleAngle = 0;
        public int GimbleAngle
        {
            get
            {
                return gimpbleAngle;
            }
            set
            {
                gimpbleAngle = value;
                RaisepropertyChanged();
            }
        }

        private double altitude;

        public double Altitude
        {
            get { return altitude; }
            set
            {
                altitude = value;
                RaisepropertyChanged();
            }
        }

        private double velocity;

        public double Velocity
        {
            get { return velocity; }
            set
            {
                velocity = value;
                RaisepropertyChanged();
            }
        }

        private double heading;

        public double Heading
        {
            get { return heading; }
            set
            {
                heading = value;
                RaisepropertyChanged();
            }
        }

        public Visibility ControlsVisible
        {
            get
            {
                if (IsConnected)
                {
                    return Visibility.Visible;
                }
                return Visibility.Collapsed;
            }
        }

        private WriteableBitmap videoSource;

        public WriteableBitmap VideoSource
        {
            get { return videoSource; }
            set {
                if (videoSource != value)
                {
                    videoSource = value;
                    RaisepropertyChanged();
                }
            }
        }


        private string recognizedObjectText;
        public string RecognizedObjectText
        {
            get { return recognizedObjectText; }
            set
            {
                recognizedObjectText = value;
                RaisepropertyChanged();
            }
        }


        private void DjiClient_FlyingChanged(bool newValue)
        {
            IsFlying = newValue;
        }

        private void DjiClient_ConnectedChanged(bool newValue)
        {
            IsConnected = newValue;
        }

        private void DjiClient_VelocityChanged(double X, double Y, double Z)
        {
            double airSpeed = X * X + Y * Y + Z * Z;
            airSpeed = Math.Abs(airSpeed) > 0.0001 ? Math.Sqrt(airSpeed) : 0;
            Velocity = airSpeed;            
        }

        private void DjiClient_AttitudeChanged(double pitch, double yaw, double roll)
        {
            Heading = yaw;
        }

        private void DjiClient_AltitudeChanged(double newValue)
        {
            Altitude = newValue;
        }

        private async void DjiClient_FrameArived(IBuffer buffer, uint width, uint height, ulong timeStamp)
        {
            if (runProcessTask == null || runProcessTask.IsCompleted)
            {
                // Do not forget to dispose it! In this sample, we dispose it in ProcessSoftwareBitmap
                SoftwareBitmap bitmapToProcess = SoftwareBitmap.CreateCopyFromBuffer(buffer,
                        BitmapPixelFormat.Bgra8, (int)width, (int)height, BitmapAlphaMode.Premultiplied);

                runProcessTask = ProcessSoftwareBitmap(bitmapToProcess, timeStamp);
                // You may want to uncomment next line if want to wait until processing is done
                //runProcessTask.Wait();
            }

            await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
            {
                if (VideoSource == null || VideoSource.PixelWidth != width || VideoSource.PixelHeight != height)
                {
                    VideoSource = new WriteableBitmap((int)width, (int)height);
                }

                buffer.CopyTo(VideoSource.PixelBuffer);

                VideoSource.Invalidate();
            });
        }


        private async Task ProcessSoftwareBitmap(SoftwareBitmap bitmap, ulong timeStamp)
        {
            // Here you can process your frame data 
            // Make sure you use Dispatcher.RunAsync for updating the UI 

            // But we will run evalustion of a WinML model 
            try
            {
                await RunModelOnBitmap(bitmap);
            }
            finally
            {
                bitmap.Dispose();
            }
        }


        private async Task RunModelOnBitmap(SoftwareBitmap bitmap)
        {
            if (!Windows.Foundation.Metadata.ApiInformation.IsMethodPresent("Windows.Media.VideoFrame", "CreateWithSoftwareBitmap"))
            {
                return;
            }

            using (VideoFrame frame = VideoFrame.CreateWithSoftwareBitmap(bitmap))
            {
                if (mlModel == null)
                {
                    var file = await Windows.Storage.StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///WinML/InkShapesModel.onnx"));
                    mlModel = await InkShapes.InkShapesModel.CreateInkShapesModel(file, 21);
                }

                InkShapes.InkShapesModelInput input = new InkShapes.InkShapesModelInput();
                input.data = frame;
                var output = await mlModel.EvaluateAsync(input);

                if (output != null)
                {

                    string recognizedTag = output.classLabel.First();
                    float recognitionConfidence = output.loss.OrderByDescending(kv => kv.Value).First().Value;

                    if (recognitionConfidence < 0.15f)
                    {
                        RecognizedObjectText = string.Empty;
                    }
                    else if (recognitionConfidence > 0.3f)
                    {
                        Debug.WriteLine("ML model evaluation result: {0} ({1})", recognizedTag, recognitionConfidence);
                        RecognizedObjectText = "Recognized: " + recognizedTag;
                    }
                }


            }
        }




    }
}
