#ifndef __otbGMMMachineLearningModel_txx
#define __otbGMMMachineLearningModel_txx

#include <fstream>
#include "itkMacro.h"
#include "itkSubsample.h"
#include "otbGMMMachineLearningModel.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
GMMMachineLearningModel<TInputValue,TOutputValue>
::GMMMachineLearningModel():
  m_classNb(0),
  m_featNb(0)
{
  m_CovarianceEstimator = CovarianceEstimatorType::New()
}


template <class TInputValue, class TOutputValue>
GMMMachineLearningModel<TInputValue,TOutputValue>
::~GMMMachineLearningModel()
{
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
GMMMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  // Get pointer to samples and labels
  typename InputListSampleType::Pointer samples = this->GetInputListSample()
  typename TargetListSampleType::Pointer labels = this->GetTargetListSample()

  // Declare iterator for samples and labels
  typename TargetListSampleType::ConstIterator refIterator;
  typename InputListSampleType::ConstIterator inputIterator;

  // Get number of samples
  unsigned long sampleNb = samples->Size();

  // Get number of features
  const MeasurementVectorSizeType measurementVectorLength = samples->GetMeasurementVectorSize();
  m_featNb = (unsigned int) measurementVectorLength;

  // Get number of classes and map indice with label
  refIterator = labels->Begin();
  while (refIterator != labels->End())
    {
      TargetSampleType currentLabel = refIterator.GetMeasurementVector()[0];
      if (m_MapOfClasses.find(currentLabel) == m_MapOfClasses.end())
        {
          m_MapOfClasses[currentLabel] = m_classNb;
          m_MapOfIndices[m_classNb] = currentLabel;
          ++m_classNb;
        }
      ++refIterator;
    }

  // Create one subsample set for each class
  typedef itk::Statistics::Subsample< InputSampleType > ClassSampleType;
  std::vector< ClassSampleType::Pointer > classSamples;
  for ( unsigned int i = 0; i < m_classNb; ++i )
    {
      classSamples.push_back( ClassSampleType::New() );
      classSamples[i]->SetSample( sample );
    }
  refIterator = labels->Begin();
  inputIterator = sample->Begin();
  while (inputIterator != sample->End())
    {
      TargetSampleType currentLabel = refIterator.GetMeasurementVector()[0];
      classSamples[m_MapOfClasses[currentLabel]]->AddInstance( inputIterator.GetInstanceIdentifier() );
      ++inputIterator;
      ++refIterator;
    }

  // Estimate covariance matrices, mean vectors and proportions
  for ( unsigned int i = 0; i < m_classNb; ++i )
    {
      m_NbSpl.push_back(classSamples[i]->Size())
      m_Proportion.push_back((unsigned float) m_NbSpl[i] / (unsigned float) sampleNb)

      m_CovarianceEstimator->SetInput( classSamples[i] );
      m_CovarianceEstimator->Update();

      m_Covariances.push_back(m_CovarianceEstimator->GetCovarianceMatrix())
      m_Means.push_back(m_CovarianceEstimator->GetMean())
    }

}

template <class TInputValue, class TOutputValue>
typename GMMMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
GMMMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & input, ConfidenceValueType *quality) const
{
  TargetSampleType target;

  // target[0] = static_cast<TOutputValue>(result);

  // if (quality != NULL)
  //   {
  //   if (!this->HasConfidenceIndex())
  //     {
  //     itkExceptionMacro("Confidence index not available for this classifier !");
  //     }
  //   }

  return target;
}

template <class TInputValue, class TOutputValue>
void
GMMMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
  // if (name == "")
  //   m_NormalBayesModel->save(filename.c_str(), 0);
  // else
  //   m_NormalBayesModel->save(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
void
GMMMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
  // if (name == "")
  // else
}

template <class TInputValue, class TOutputValue>
bool
GMMMachineLearningModel<TInputValue,TOutputValue>
::CanReadFile(const std::string & file)
{
  // std::ifstream ifs;
  // ifs.open(file.c_str());

  // if(!ifs)
  // {
  //   std::cerr<<"Could not read file "<<file<<std::endl;
  //   return false;
  // }

  // while (!ifs.eof())
  // {
  //   std::string line;
  //   std::getline(ifs, line);

  //   if (line.find(CV_TYPE_NAME_ML_NBAYES) != std::string::npos)
  //   {
  //      //std::cout<<"Reading a "<<CV_TYPE_NAME_ML_NBAYES<<" model"<<std::endl;
  //      return true;
  //   }
  // }
  // ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool
GMMMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void
GMMMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif