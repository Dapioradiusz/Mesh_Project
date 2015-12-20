#include "GridGui.h"
#include <stdlib.h>

using namespace System;
using namespace System::Windows::Forms;

grid_gui::GridGui::GridGui() :
	xbeeThreadStarted(false),
	allowDataSend(false)
{
	InitializeComponent();

	getAvailableNetworks();
	dataToSend = new std::vector<std::string>; // data buffer
	sensors = new std::vector<SmartSensor>;
	//worker thread initialization
	xbeeBackGroundWorker->WorkerSupportsCancellation = true;
	xbeeBackGroundWorker->DoWork += gcnew DoWorkEventHandler(this, &grid_gui::GridGui::xbeeBackGroundWorker_DoWork);
	//initialize picture box 
	pbSensorMap->Size = System::Drawing::Size(490, 320);
	pbSensorMap->BackgroundImageLayout = ImageLayout::Stretch;
}

grid_gui::GridGui::~GridGui()
{
	if (components)
	{
		delete components;
	}
}

void grid_gui::GridGui::InitializeComponent()
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
	this->btnRequestMeasure = (gcnew System::Windows::Forms::Button());
	this->btnAddSensor = (gcnew System::Windows::Forms::Button());
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
	this->btnConnect->Click += gcnew System::EventHandler(this, &GridGui::btnConnect_Click);
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
	this->btnSendData->Click += gcnew System::EventHandler(this, &GridGui::btnSendData_Click);
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
	// 
	// btnRequestMeasure
	// 
	this->btnRequestMeasure->Location = System::Drawing::Point(842, 550);
	this->btnRequestMeasure->Name = L"btnRequestMeasure";
	this->btnRequestMeasure->Size = System::Drawing::Size(124, 50);
	this->btnRequestMeasure->TabIndex = 12;
	this->btnRequestMeasure->Text = L"Request Measurment";
	this->btnRequestMeasure->UseVisualStyleBackColor = true;
	// 
	// btnAddSensor
	// 
	this->btnAddSensor->Location = System::Drawing::Point(608, 606);
	this->btnAddSensor->Name = L"btnAddSensor";
	this->btnAddSensor->Size = System::Drawing::Size(197, 50);
	this->btnAddSensor->TabIndex = 13;
	this->btnAddSensor->Text = L"Add Sensor";
	this->btnAddSensor->UseVisualStyleBackColor = true;
	this->btnAddSensor->Click += gcnew System::EventHandler(this, &GridGui::btnAddSensor_Click);
	// 
	// GridGui
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(1428, 861);
	this->Controls->Add(this->btnAddSensor);
	this->Controls->Add(this->btnRequestMeasure);
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
	this->Name = L"GridGui";
	this->Text = L"Grid GUI";
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSensorMap))->EndInit();
	this->ResumeLayout(false);
	this->PerformLayout();

}

System::Void grid_gui::GridGui::btnConnect_Click(System::Object^  sender, System::EventArgs^  e)
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

System::Void grid_gui::GridGui::btnSendData_Click(System::Object^  sender, System::EventArgs^  e)
{
	if (dataToSend->size() == MAX_BUFFER_SIZE)
	{
		dataToSend->pop_back(); //erases previously send data
	}
	dataToSend->push_back("GUI Data");//instead of strings can later store data objects
}

System::Void grid_gui::GridGui::btnAddSensor_Click(System::Object^  sender, System::EventArgs^  e)
{
	std::vector<uint64_t> sensorIds;
	this->Hide();
	dialog::SensorDialog ^sensorCreator = gcnew dialog::SensorDialog(&sensorIds, sensors);
	sensorCreator->ShowDialog();
	this->Show();
	if (!sensorIds.empty())
	{
		for (std::vector<uint64_t>::const_iterator itr = sensorIds.begin(); itr != sensorIds.end(); itr++)
		{
			this->cbSensorParamsList->Items->Add("Sensor" + Convert::ToString(*itr));
		}
		//redraw the map - 
	}

}

bool grid_gui::GridGui::getAvailableNetworks()
{
	// just for testing - this method should take
	// info from attached xbee module to HUB
	this->cbNetList->Items->Add("Network 1");
	this->cbNetList->Items->Add("Network 2");

	return true;
}

void grid_gui::GridGui::produceAllow(bool allow)
{
	this->btnSendData->Enabled = allow;
}

void grid_gui::GridGui::onReceivedData(std::istringstream &frameData)
{
	//reads frameData content to 
	frameData.seekg(0, frameData.end);
	size_t length = frameData.tellg();
	frameData.seekg(0, frameData.beg);

	// allocate memory:
	char *buffer = new char[length];

	// read data as a block:
	frameData.read(buffer, length);
	System::String ^data = gcnew System::String(buffer);
	this->txLog->AppendText("Received data : " + data + "\n");
	
	//test for logging sensors and drawing map
	//add new measurments to sensors based on the data frame
	if (actualizeSensorData(frameData))
	{
		//actualize sensor  map
		drawMap();
	}
	
	delete buffer;
}

void grid_gui::GridGui::onDataSend(System::String ^data)
{
	this->txLog->AppendText("Send data : " + data + "\n");
}

System::Void grid_gui::GridGui::xbeeBackGroundWorker_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
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

		this->Invoke(gcnew semaphoreDelegate(this, &grid_gui::GridGui::produceAllow), canProduce); // dont produce while it reads
		//in future make it thread safe -  if there are multiple consumers, they can't consume data at the same time
		if (!dataToSend->empty())
		{
			System::String ^writeData = gcnew System::String((*dataToSend)[dataToSend->size() - 1].c_str());
			dataToSend->erase(dataToSend->end() - 1); // removes consumed data
			this->Invoke(gcnew dataSendDelegate(this, &grid_gui::GridGui::onDataSend), writeData);
			//send it using xbee digi/zigbee api by serial port / or send it to xbee handler 
		}

		canProduce = true;
		this->Invoke(gcnew semaphoreDelegate(this, &grid_gui::GridGui::produceAllow), canProduce); // data read, now can produce

		// now check if received data - > maybe later sending and reading on separate threads
		//if received new frame invoke the onReceivedData() function
		//for now read the test_frame.xml file to test the data actualization and map drawing
		//read file to isstringstream
		this->Invoke(gcnew receivedDataDelegate(this, &grid_gui::GridGui::onReceivedData), "xbee data"); // just testing

		Thread::Sleep(500);
	}
}

System::Drawing::Bitmap^ grid_gui::GridGui::MatToBitmap(const cv::Mat& img)
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

bool grid_gui::GridGui::actualizeSensorData(std::istringstream &frameData)
{
	//later wil use a proper xml processing library - for now only basic stuff for testing
	bool updatedSensors = false;
	for (std::string line; std::getline(frameData, line);)
	{
		if (line == "<sensor>")
		{
			//extract id
			std::getline(frameData, line);  //gets <id>...</id>
			uint64_t frame_id = atoi(extractTagMember(line).c_str());
			//look if this sensor is in sensors list
			size_t sensorNr = findSensorByID(frame_id);
			if (sensorNr >= 0)
			{
				std::getline(frameData, line);
				while (line != "</sensor>")
				{
					if (line == "<measurment>")
					{
						std::getline(frameData, line); // gets <name>...</name>	
						std::string meas_name = extractTagMember(line);
						std::getline(frameData, line);//gets <value>...</value>
						double meas_value = atof(extractTagMember(line).c_str());
						//add new measurment
						(*sensors)[sensorNr].m_measurments.push_back(std::make_pair(meas_name, meas_value));
						updatedSensors = true;
					}
					std::getline(frameData, line);
				}
			}
		}
	}
	return updatedSensors;
}

size_t grid_gui::GridGui::findSensorByID(uint64_t id)
{
	size_t sensorNr = 0;
	for (std::vector<SmartSensor>::const_iterator itr = sensors->begin(); itr != sensors->end(); itr++, sensorNr++)
	{
		if (itr->getId() == id)
		{
			return sensorNr;
		}
	}
	return -1;
}

std::string grid_gui::GridGui::extractTagMember(const std::string &line)
{
	size_t first = line.find_first_of(">");
	size_t second = line.find_first_of("<", first + 1);
	return std::string(line.begin() + first + 1, line.begin() + second);
}

void grid_gui::GridGui::drawMap()
{
	SensorMap map;
	Mat matImage = map.getMap(*sensors);
	Bitmap^ btMap = MatToBitmap(matImage);
	pbSensorMap->BackgroundImage = btMap;
	pbSensorMap->BackgroundImageLayout = ImageLayout::Stretch;
}

[STAThread]
void Main(array<System::String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	grid_gui::GridGui form;
	Application::Run(%form);
}