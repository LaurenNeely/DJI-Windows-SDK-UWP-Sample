using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Windows.Media;
using Windows.Storage;
using Windows.AI.MachineLearning.Preview;

// ApplesAndBananas

namespace ApplesAndBananas
{
    public sealed class ApplesAndBananasModelInput
    {
        public VideoFrame data { get; set; }
    }

    public sealed class ApplesAndBananasModelOutput
    {
        public IList<string> classLabel { get; set; }
        public IDictionary<string, float> loss { get; set; }
        public ApplesAndBananasModelOutput()
        {
            this.classLabel = new List<string>();
            this.loss = new Dictionary<string, float>()
            {
                { "Apple", float.NaN },
                { "Banana", float.NaN },
            };
        }
    }

    public sealed class ApplesAndBananasModel
    {
        private LearningModelPreview learningModel;
        public static async Task<ApplesAndBananasModel> CreateApplesAndBananasModel(StorageFile file)
        {
            LearningModelPreview learningModel = await LearningModelPreview.LoadModelFromStorageFileAsync(file);
            ApplesAndBananasModel model = new ApplesAndBananasModel();
            model.learningModel = learningModel;
            return model;
        }
        public async Task<ApplesAndBananasModelOutput> EvaluateAsync(ApplesAndBananasModelInput input) {
            ApplesAndBananasModelOutput output = new ApplesAndBananasModelOutput();
            LearningModelBindingPreview binding = new LearningModelBindingPreview(learningModel);
            binding.Bind("data", input.data);
            binding.Bind("classLabel", output.classLabel);
            binding.Bind("loss", output.loss);
            LearningModelEvaluationResultPreview evalResult = await learningModel.EvaluateAsync(binding, string.Empty);
            return output;
        }
    }
}
