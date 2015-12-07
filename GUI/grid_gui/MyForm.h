#pragma once
#include "SmartSensor.h"
#include "SensorMap.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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
	/// Summary for MyForm
	/// </summary>
	delegate void semaphoreDelegate(bool allowDataSend);
	delegate void dataSendDelegate(System::String ^data);
	delegate void receivedDataDelegate(System::String ^data);

	const uint8_t MAX_BUFFER_SIZE = 100;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	private:
		bool xbeeThreadStarted;
		bool allowDataSend;
		std::vector<std::string > *dataToSend; //later replaced by vector of frames
	private: System::Windows::Forms::Button^  btnSendData;
	private: System::Windows::Forms::TextBox^  txSensorInfo;
	private: System::Windows::Forms::Button^  btnSensorMode;
	private: System::Windows::Forms::Button^  button1;




	private: System::ComponentModel::BackgroundWorker^  xbeeBackGroundWorker;

	public:
		MyForm(void) :
			xbeeThreadStarted(false),
			allowDataSend(false)
		{
			InitializeComponent();

			getAvailableNetworks();
			dataToSend = new std::vector < std::string > ; // data buffer
			//worker thread initialization
			xbeeBackGroundWorker->WorkerSupportsCancellation = true;
			xbeeBackGroundWorker->DoWork += gcnew DoWorkEventHandler(this, &grid_gui::MyForm::xbeeBackGroundWorker_DoWork);

			// testing displaying opencv images in picturebox control
			std::vector<std::pair<std::string, double> > meas1, meas2;
			meas1.push_back(std::make_pair("temperature", 21.7));
			meas1.push_back(std::make_pair("humidity", 59.2));
			meas2.push_back(std::make_pair("temperature", 22.3));
			meas2.push_back(std::make_pair("humidity", 62.7));

			SmartSensor sensor1(1, std::make_pair(53.419429, 14.436434), 100, meas1);
			SmartSensor sensor2(2, std::make_pair(53.422053, 14.435080), 100, meas2);
			//create vector of sensors - normally would be created by looping over all incoming data
			std::vector<SmartSensor> sensorList = { sensor1, sensor2 };
			SensorMap map;
			Mat matImage = map.getMap(sensorList);
			pbSensorMap->Size = System::Drawing::Size(490, 320);
			Bitmap^ btMap = MatToBitmap(matImage);
			pbSensorMap->BackgroundImage = btMap;
			pbSensorMap->BackgroundImageLayout = ImageLayout::Stretch;		
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ComboBox^  cbNetList;
	private: System::Windows::Forms::PictureBox^  pbSensorMap;
	protected:

	protected:

	private: System::Windows::Forms::ComboBox^  cbSensorParamsList;

	private: System::Windows::Forms::Button^  btnConnect;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  txLog;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;


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
		void InitializeComponent(void)
		{
			this->cbNetList = (gcnew System::Windows::Forms::ComboBox());
			this->pbSensorMap = (gcnew System::Windows::Forms::PictureBox());
			this->cbSensorParamsList = (gcnew System::Windows::Forms::ComboBox());
			this->btnConnect = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txLog = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->xbeeBackGroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->btnSendData = (gcnew System::Windows::Forms::Button());
			this->txSensorInfo = (gcnew System::Windows::Forms::TextBox());
			this->btnSensorMode = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSensorMap))->BeginInit();
			this->SuspendLayout();
			// 
			// cbNetList
			// 
			this->cbNetList->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbNetList->FormattingEnabled = true;
			this->cbNetList->ImeMode = System::Windows::Forms::ImeMode::NoControl;
			this->cbNetList->Location = System::Drawing::Point(52, 440);
			this->cbNetList->Name = L"cbNetList";
			this->cbNetList->Size = System::Drawing::Size(180, 21);
			this->cbNetList->TabIndex = 0;
			this->cbNetList->TabStop = false;
			// 
			// pbSensorMap
			// 
			this->pbSensorMap->Location = System::Drawing::Point(52, 66);
			this->pbSensorMap->Name = L"pbSensorMap";
			this->pbSensorMap->Size = System::Drawing::Size(490, 330);
			this->pbSensorMap->TabIndex = 1;
			this->pbSensorMap->TabStop = false;
			// 
			// cbSensorParamsList
			// 
			this->cbSensorParamsList->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbSensorParamsList->FormattingEnabled = true;
			this->cbSensorParamsList->Location = System::Drawing::Point(608, 440);
			this->cbSensorParamsList->Name = L"cbSensorParamsList";
			this->cbSensorParamsList->Size = System::Drawing::Size(180, 21);
			this->cbSensorParamsList->TabIndex = 2;
			// 
			// btnConnect
			// 
			this->btnConnect->Location = System::Drawing::Point(52, 500);
			this->btnConnect->Name = L"btnConnect";
			this->btnConnect->Size = System::Drawing::Size(179, 50);
			this->btnConnect->TabIndex = 3;
			this->btnConnect->Text = L"Connect/Disconnect";
			this->btnConnect->UseVisualStyleBackColor = true;
			this->btnConnect->Click += gcnew System::EventHandler(this, &MyForm::btnConnect_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(82, 423);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(117, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Available Networks List";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(605, 410);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(45, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Sensors";
			this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click);
			// 
			// txLog
			// 
			this->txLog->Location = System::Drawing::Point(608, 66);
			this->txLog->Multiline = true;
			this->txLog->Name = L"txLog";
			this->txLog->ReadOnly = true;
			this->txLog->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txLog->Size = System::Drawing::Size(376, 329);
			this->txLog->TabIndex = 6;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(752, 34);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(82, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Connection Log";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(243, 34);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(64, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Sensor Map";
			// 
			// btnSendData
			// 
			this->btnSendData->Location = System::Drawing::Point(316, 500);
			this->btnSendData->Name = L"btnSendData";
			this->btnSendData->Size = System::Drawing::Size(179, 50);
			this->btnSendData->TabIndex = 9;
			this->btnSendData->Text = L"Send sample";
			this->btnSendData->UseVisualStyleBackColor = true;
			this->btnSendData->Click += gcnew System::EventHandler(this, &MyForm::btnSendData_Click);
			// 
			// txSensorInfo
			// 
			this->txSensorInfo->Location = System::Drawing::Point(608, 477);
			this->txSensorInfo->Multiline = true;
			this->txSensorInfo->Name = L"txSensorInfo";
			this->txSensorInfo->ReadOnly = true;
			this->txSensorInfo->Size = System::Drawing::Size(197, 123);
			this->txSensorInfo->TabIndex = 10;
			this->txSensorInfo->Visible = false;
			// 
			// btnSensorMode
			// 
			this->btnSensorMode->Location = System::Drawing::Point(842, 477);
			this->btnSensorMode->Name = L"btnSensorMode";
			this->btnSensorMode->Size = System::Drawing::Size(124, 50);
			this->btnSensorMode->TabIndex = 11;
			this->btnSensorMode->Text = L"Sleep/Wake";
			this->btnSensorMode->UseVisualStyleBackColor = true;
			this->btnSensorMode->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(842, 550);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(124, 50);
			this->button1->TabIndex = 12;
			this->button1->Text = L"Request Measurment";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1428, 861);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->btnSensorMode);
			this->Controls->Add(this->txSensorInfo);
			this->Controls->Add(this->btnSendData);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->txLog);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btnConnect);
			this->Controls->Add(this->cbSensorParamsList);
			this->Controls->Add(this->pbSensorMap);
			this->Controls->Add(this->cbNetList);
			this->Name = L"MyForm";
			this->Text = L"Grid GUI";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSensorMap))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
		System::Void btnConnect_Click(System::Object^  sender, System::EventArgs^  e)
		{
			// connect button code - etstablishes connection using XbeeHandler class
			// creates or informs separate thread on starting communication beetween
			// selected network <-> XbeeModule <-> XbeeThread(worker thread) <-> Gui(main thread)
			if (!xbeeThreadStarted)
			{
				//start the xbee communication	
				xbeeThreadStarted = true;
				xbeeBackGroundWorker->RunWorkerAsync();
			}
			else
			{
				xbeeThreadStarted = false;
				xbeeBackGroundWorker->CancelAsync();
			}
		}

		System::Void btnSendData_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if (dataToSend->size() == MAX_BUFFER_SIZE)
			{
				dataToSend->pop_back(); //erases previously send data
			}
			dataToSend->push_back("GUI Data");//instead of strings can later store data objects
		}

		bool getAvailableNetworks()
		{
			// just for testing - this method should take
			// info from attached xbee module to HUB
			this->cbNetList->Items->Add("Network 1");
			this->cbNetList->Items->Add("Network 2");

			return true;
		}

		void produceAllow(bool allow)
		{
			this->btnSendData->Enabled = allow;
		}

		void onReceivedData(System::String ^data)
		{
			this->txLog->AppendText("Received data : " + data + "\n");
			//test for logging sensors and drawing map
			std::vector<std::pair<std::string, double> > meas1, meas2;
		}

		void onDataSend(System::String ^data)
		{
			this->txLog->AppendText("Send data : " + data + "\n");
		}

		System::Void xbeeBackGroundWorker_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
		{
			while (true)
			{
				//check if thread cancelled
				if (xbeeBackGroundWorker->CancellationPending)
				{
					e->Cancel = true;
					break;
				}

				bool canProduce = false;

				// Note your object array size will depend on the number of arguments you need to pass
				/*The delegate inturns calls producerAllow function with the passed arguements
				The producerAllow function can easily modify the command button because it   runs on the User Interface thread ( The default thread on which the button was created)*/

				this->Invoke(gcnew semaphoreDelegate(this, &grid_gui::MyForm::produceAllow), canProduce); // dont produce while it reads
				//in future make it thread safe -  if thera are multiple consumers, they can't consume data at the same time
				if (!dataToSend->empty())
				{
					System::String ^writeData = gcnew System::String((*dataToSend)[dataToSend->size() - 1].c_str());
					dataToSend->erase(dataToSend->end() - 1); // removes consumed data
					this->Invoke(gcnew dataSendDelegate(this, &grid_gui::MyForm::onDataSend), writeData);
					//send it using xbee digi/zigbee api by serial port / or send it to xbee handler to 
				}

				canProduce = true;
				this->Invoke(gcnew semaphoreDelegate(this, &grid_gui::MyForm::produceAllow), canProduce); // data read, now can produce

				// now check if received data - > maybe later sending and reading on separate threads
				this->Invoke(gcnew receivedDataDelegate(this, &grid_gui::MyForm::onReceivedData), "xbee data"); // just testing

				Thread::Sleep(500);
			}
		}

		System::Drawing::Bitmap^ MatToBitmap(const cv::Mat& img)
		{
			if (img.type() != CV_8UC3)
			{
				throw gcnew NotSupportedException("Only images of type CV_8UC3 are supported for conversion to Bitmap");
			}

			//create the bitmap and get the pointer to the data
			PixelFormat fmt(PixelFormat::Format24bppRgb);
			Bitmap ^bmpimg = gcnew Bitmap(img.cols, img.rows, fmt);

			BitmapData ^data = bmpimg->LockBits(System::Drawing::Rectangle(0, 0, img.cols, img.rows), ImageLockMode::WriteOnly, fmt);

			char *dstData = reinterpret_cast<char*>(data->Scan0.ToPointer());

			unsigned char *srcData = img.data;

			for (int row = 0; row < data->Height; ++row)
			{
				memcpy(reinterpret_cast<void*>(&dstData[row*data->Stride]), reinterpret_cast<void*>(&srcData[row*img.step]), img.cols*img.channels());
			}

			bmpimg->UnlockBits(data);

			return bmpimg;
		}
	
		
	private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
	}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
}
};

}
