#pragma once
#include "SmartSensor.h"

namespace dialog {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for SensorDialog
	/// </summary>

	public ref class SensorDialog : public System::Windows::Forms::Form
	{
	private:
		std::vector<SmartSensor> *m_Sensors;
		std::vector<uint64_t> *m_sensorIds;

		//ui controls
		System::Windows::Forms::Label^  label1;
		System::Windows::Forms::Label^  label2;
		System::Windows::Forms::Label^  label3;
		System::Windows::Forms::Label^  label4;

		System::Windows::Forms::TextBox^  txSensorId;
		System::Windows::Forms::TextBox^  txPosLat;
		System::Windows::Forms::TextBox^  txPosLong;
		System::Windows::Forms::TextBox^  txRange;

		System::Windows::Forms::Button^  btnCancel;
		System::Windows::Forms::Button^  btnAddSensor;

	public:
		SensorDialog();

		SensorDialog(std::vector<uint64_t> *sensorIds, std::vector<SmartSensor> *Sensors);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SensorDialog();
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
		void InitializeComponent();
#pragma endregion

		System::Void btnAddSensor_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btnCancel_Click(System::Object^  sender, System::EventArgs^  e);

		bool checkIfDuplicate(const SmartSensor &sensor);
	
	
	};
}
