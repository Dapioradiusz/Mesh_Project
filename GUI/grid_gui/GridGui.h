#pragma once
#include "SmartSensor.h"
#include "SensorMap.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "SensorDialog.h"

namespace grid_gui {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;
	using namespace System::Threading; // for working with threads
	using namespace cv;

	/// <summary>
	/// Summary for GridGui
	/// </summary>
	delegate void semaphoreDelegate(bool allowDataSend);
	delegate void dataSendDelegate(System::String ^data);
	delegate void receivedDataDelegate(System::String ^data);
	delegate void addedSensor(uint64_t id);

	const uint8_t MAX_BUFFER_SIZE = 100;

	public ref class GridGui : public System::Windows::Forms::Form
	{
	private:
		bool xbeeThreadStarted;
		bool allowDataSend;
		std::vector<std::string> *dataToSend; //later replaced by vector of frames
		std::vector<SmartSensor> *sensors;

		//ui controls
		System::Windows::Forms::Label^  label1;
		System::Windows::Forms::Label^  label2;
		System::Windows::Forms::Label^  label3;
		System::Windows::Forms::Label^  label4;

		System::Windows::Forms::TextBox^  txLog;
		System::Windows::Forms::TextBox^  txSensorInfo;
		
		System::Windows::Forms::Button^  btnSendData;		
		System::Windows::Forms::Button^  btnSensorMode;
		System::Windows::Forms::Button^  btnConnect;
	    System::Windows::Forms::Button^  btnRequestMeasure;

		System::Windows::Forms::Button^  btnAddSensor;

		System::Windows::Forms::ComboBox^  cbNetList;
		System::Windows::Forms::ComboBox^  cbSensorParamsList;

		System::Windows::Forms::PictureBox^  pbSensorMap;

		System::ComponentModel::BackgroundWorker^  xbeeBackGroundWorker;
		
	public:
		GridGui();
	protected:
		~GridGui();
	
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
	private:
		void InitializeComponent();
		
		System::Void btnConnect_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btnSendData_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btnAddSensor_Click(System::Object^  sender, System::EventArgs^  e);

		System::Void cbSensorParamsList_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);

		bool getAvailableNetworks();

		void produceAllow(bool allow);
		void onReceivedData(System::String ^frameData);
		void onDataSend(System::String ^data);

		System::Void xbeeBackGroundWorker_DoWork(System::Object^  sender, 
												 System::ComponentModel::DoWorkEventArgs^  e);

		System::Drawing::Bitmap^ MatToBitmap(const cv::Mat& img);
		
		bool actualizeSensorData(std::stringstream &frameData);
		bool findSensorByID(uint64_t id, size_t &sensorNr);
		std::string extractTagMember(const std::string &line);
		
		void drawMap();
		
	
	};

}
