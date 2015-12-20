#include "SensorDialog.h"

dialog::SensorDialog::SensorDialog()
{
	InitializeComponent();
}

dialog::SensorDialog::SensorDialog(std::vector<uint64_t> *sensorIds, std::vector<SmartSensor> *Sensors) :
m_sensorIds(sensorIds),
m_Sensors(Sensors)
{
	InitializeComponent();
}

dialog::SensorDialog::~SensorDialog()
{
	if (components)
	{
		delete components;
	}
}

void dialog::SensorDialog::InitializeComponent(void)
{
	this->btnAddSensor = (gcnew System::Windows::Forms::Button());
	this->label1 = (gcnew System::Windows::Forms::Label());
	this->txSensorId = (gcnew System::Windows::Forms::TextBox());
	this->label2 = (gcnew System::Windows::Forms::Label());
	this->label3 = (gcnew System::Windows::Forms::Label());
	this->label4 = (gcnew System::Windows::Forms::Label());
	this->txPosLat = (gcnew System::Windows::Forms::TextBox());
	this->txPosLong = (gcnew System::Windows::Forms::TextBox());
	this->txRange = (gcnew System::Windows::Forms::TextBox());
	this->btnCancel = (gcnew System::Windows::Forms::Button());
	this->SuspendLayout();
	// 
	// btnAddSensor
	// 
	this->btnAddSensor->Location = System::Drawing::Point(48, 301);
	this->btnAddSensor->Name = L"btnAddSensor";
	this->btnAddSensor->Size = System::Drawing::Size(75, 23);
	this->btnAddSensor->TabIndex = 0;
	this->btnAddSensor->Text = L"Add";
	this->btnAddSensor->UseVisualStyleBackColor = true;
	this->btnAddSensor->Click += gcnew System::EventHandler(this, &SensorDialog::btnAddSensor_Click);
	// 
	// label1
	// 
	this->label1->AutoSize = true;
	this->label1->Location = System::Drawing::Point(45, 44);
	this->label1->Name = L"label1";
	this->label1->Size = System::Drawing::Size(80, 13);
	this->label1->TabIndex = 1;
	this->label1->Text = L"Sensor id(MAC)";
	// 
	// txSensorId
	// 
	this->txSensorId->Location = System::Drawing::Point(149, 44);
	this->txSensorId->Multiline = true;
	this->txSensorId->Name = L"txSensorId";
	this->txSensorId->Size = System::Drawing::Size(219, 67);
	this->txSensorId->TabIndex = 2;
	// 
	// label2
	// 
	this->label2->AutoSize = true;
	this->label2->Location = System::Drawing::Point(45, 126);
	this->label2->Name = L"label2";
	this->label2->Size = System::Drawing::Size(85, 13);
	this->label2->TabIndex = 3;
	this->label2->Text = L"Position Latitude";
	// 
	// label3
	// 
	this->label3->AutoSize = true;
	this->label3->Location = System::Drawing::Point(45, 212);
	this->label3->Name = L"label3";
	this->label3->Size = System::Drawing::Size(39, 13);
	this->label3->TabIndex = 4;
	this->label3->Text = L"Range";
	// 
	// label4
	// 
	this->label4->AutoSize = true;
	this->label4->Location = System::Drawing::Point(45, 163);
	this->label4->Name = L"label4";
	this->label4->Size = System::Drawing::Size(94, 13);
	this->label4->TabIndex = 5;
	this->label4->Text = L"Position Longitude";
	// 
	// txPosLat
	// 
	this->txPosLat->Location = System::Drawing::Point(149, 126);
	this->txPosLat->Name = L"txPosLat";
	this->txPosLat->Size = System::Drawing::Size(100, 20);
	this->txPosLat->TabIndex = 6;
	// 
	// txPosLong
	// 
	this->txPosLong->Location = System::Drawing::Point(149, 163);
	this->txPosLong->Name = L"txPosLong";
	this->txPosLong->Size = System::Drawing::Size(100, 20);
	this->txPosLong->TabIndex = 7;
	// 
	// txRange
	// 
	this->txRange->Location = System::Drawing::Point(149, 205);
	this->txRange->Name = L"txRange";
	this->txRange->Size = System::Drawing::Size(100, 20);
	this->txRange->TabIndex = 8;
	// 
	// btnCancel
	// 
	this->btnCancel->Location = System::Drawing::Point(293, 301);
	this->btnCancel->Name = L"btnCancel";
	this->btnCancel->Size = System::Drawing::Size(75, 23);
	this->btnCancel->TabIndex = 9;
	this->btnCancel->Text = L"Cancel";
	this->btnCancel->UseVisualStyleBackColor = true;
	this->btnCancel->Click += gcnew System::EventHandler(this, &SensorDialog::btnCancel_Click);
	// 
	// SensorDialog
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(427, 349);
	this->Controls->Add(this->btnCancel);
	this->Controls->Add(this->txRange);
	this->Controls->Add(this->txPosLong);
	this->Controls->Add(this->txPosLat);
	this->Controls->Add(this->label4);
	this->Controls->Add(this->label3);
	this->Controls->Add(this->label2);
	this->Controls->Add(this->txSensorId);
	this->Controls->Add(this->label1);
	this->Controls->Add(this->btnAddSensor);
	this->Name = L"SensorDialog";
	this->Text = L"Add Sensor";
	this->ResumeLayout(false);
	this->PerformLayout();

}

System::Void dialog::SensorDialog::btnAddSensor_Click(System::Object^  sender, System::EventArgs^  e)
{
	std::vector<std::pair<std::string, double> > meas;

	uint64_t id = Convert::ToUInt64(txSensorId->Text);
	double lat = Convert::ToDouble(txPosLat->Text);
	double lng = Convert::ToDouble(txPosLong->Text);
	double range = Convert::ToDouble(txRange->Text);

	SmartSensor sensor(Convert::ToUInt64(txSensorId->Text),
		std::make_pair(Convert::ToDouble(txPosLat->Text),
		Convert::ToDouble(txPosLong->Text)),
		Convert::ToDouble(txRange->Text), meas);

	//do a check if there is a sensor with the same id so that it isnt added more than once
	if (!checkIfDuplicate(sensor))
	{
		m_Sensors->push_back(sensor);
		m_sensorIds->push_back(Convert::ToUInt64(txSensorId->Text));
	}
}
System::Void dialog::SensorDialog::btnCancel_Click(System::Object^  sender, System::EventArgs^  e)
{
	this->Close();
}

bool dialog::SensorDialog::checkIfDuplicate(const SmartSensor &sensor)
{
	for (std::vector<SmartSensor>::const_iterator itr = m_Sensors->begin(); itr != m_Sensors->end(); itr++)
	{
		if (sensor.getId() == itr->getId())
		{
			return true;
		}
	}
	return false;
}

