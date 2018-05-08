using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Windows.Media;
using Windows.Storage;
using Windows.AI.MachineLearning.Preview;

// InkShapes

namespace InkShapes
{
    public sealed class InkShapesModelInput
    {
        public VideoFrame data { get; set; }
    }

    public sealed class InkShapesModelOutput
    {
        public IList<string> classLabel { get; set; }
        public IDictionary<string, float> loss { get; set; }
        public InkShapesModelOutput(int lossCount)
        {
            this.classLabel = new List<string>();
            this.loss = new Dictionary<string, float>();
            for (int i = 0; i < lossCount; i++)
            {
                this.loss.Add(i.ToString(), float.NaN);
            }
        }
    }

    public sealed class InkShapesModel
    {
        private int _lossCount;

        private LearningModelPreview learningModel;
        public static async Task<InkShapesModel> CreateInkShapesModel(StorageFile file, int lossCount)
        {
            LearningModelPreview learningModel = await LearningModelPreview.LoadModelFromStorageFileAsync(file);
            InkShapesModel model = new InkShapesModel();
            model.learningModel = learningModel;
            model._lossCount = lossCount;
            return model;
        }
        public async Task<InkShapesModelOutput> EvaluateAsync(InkShapesModelInput input) {
            InkShapesModelOutput output = new InkShapesModelOutput(_lossCount);
            LearningModelBindingPreview binding = new LearningModelBindingPreview(learningModel);
            binding.Bind("data", input.data);
            binding.Bind("classLabel", output.classLabel);
            binding.Bind("loss", output.loss);
            LearningModelEvaluationResultPreview evalResult = await learningModel.EvaluateAsync(binding, string.Empty);
            return output;
        }
    }
}
