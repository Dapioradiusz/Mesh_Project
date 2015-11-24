#pragma once
#include "SmartSensor.h"

namespace grid_gui {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading; // for working with threads

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	delegate void semaphoreDelegate(bool allowDataSend);
	delegate void dataSendDelegate(String ^data);
	delegate void receivedDataDelegate(String ^data);

	const uint8_t MAX_BUFFER_SIZE = 100;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	private :
		bool xbeeThreadStarted;
		bool allowDataSend;
		std::vector<std::string > *dataToSend; //later replaced by vector of frames
	private: System::Windows::Forms::Button^  btnSendData;



	private: System::ComponentModel::BackgroundWorker^  xbeeBackGroundWorker;

	public:
		MyForm(void):
			xbeeThreadStarted(false),
			allowDataSend(false)
		{
			InitializeComponent();
			
			getAvailableNetworks();
			dataToSend = new std::vector<std::string>; // data buffer
			xbeeBackGroundWorker->WorkerSupportsCancellation = true;
			xbeeBackGroundWorker->DoWork += gcnew DoWorkEventHandler(this, &grid_gui::MyForm::xbeeBackGroundWorker_DoWork);
			//initialize xbee thread
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
			this->pbSensorMap->Size = System::Drawing::Size(482, 329);
			this->pbSensorMap->TabIndex = 1;
			this->pbSensorMap->TabStop = false;
			// 
			// cbSensorParamsList
			// 
			this->cbSensorParamsList->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbSensorParamsList->FormattingEnabled = true;
			this->cbSensorParamsList->Location = System::Drawing::Point(354, 440);
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
			this->label2->Location = System::Drawing::Point(417, 423);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(96, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Sensor Parameters";
			// 
			// txLog
			// 
			this->txLog->Location = System::Drawing::Point(608, 66);
			this->txLog->Multiline = true;
			this->txLog->Name = L"txLog";
			this->txLog->ReadOnly = true;
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
			this->btnSendData->Location = System::Drawing::Point(355, 500);
			this->btnSendData->Name = L"btnSendData";
			this->btnSendData->Size = System::Drawing::Size(179, 50);
			this->btnSendData->TabIndex = 9;
			this->btnSendData->Text = L"Send sample";
			this->btnSendData->UseVisualStyleBackColor = true;
			this->btnSendData->Click += gcnew System::EventHandler(this, &MyForm::btnSendData_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1064, 639);
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
			this->Text = L"MyForm";
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
				/*xbeeThread->Stop = true;*/
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

		void onReceivedData(String ^data)
		{
			this->txLog->Text = this->txLog->Text + "Received data : " + data + "\n";
		}

		void onDataSend(String ^data)
		{
			this->txLog->Text = this->txLog->Text + "Send data : " + data + "\n";
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
					String ^writeData = gcnew String((*dataToSend)[dataToSend->size() - 1].c_str());
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
		
};

}
