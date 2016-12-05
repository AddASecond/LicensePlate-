//#include<opencv.hpp>
#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<string>

#pragma once

namespace LicensePlate {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	//必须写在托管类外面，方便调用cvRelease释放图片
	IplImage* img; //openCV image 
	IplImage* img_small; //openCV image resized to show in picturebox1
	IplImage* img_origin; //剪裁输出时用的原图
	IplImage* img_output; //输出时用的截过的图

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
#pragma region Variables and Functions
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		// add my vars here 2016
		int PicCount = 0;
		int labelCount = 0;// to save how many pictures' 5 points are set
		int TempPicNum = 0;
		int L;
		int ResizeOrNot = 0; //to indicate whether MyForm is resized or not
		int tmpLocX;
		int tmpLocY;
		int directoryIndicate = 0;
		int SelectedRow = 0;
		double SegRatioX = 0.1; //ratio that do not use
		double SegRatioY = 0.4; //ratio that do not use
		bool FormLoad = 0;//判断是否是窗体初次加载
		int IndexShift = 0;//记录序号偏移，防止若跳过一个图片，导出时序号不连续
								//CvMemStorage* storage = 0;
		array<double, 2>^ LicensePlateLocation = gcnew array<double, 2>(200000, 10); //to save license plate location
		array<double, 2>^ LPL_small = gcnew array<double, 2>(200000, 10); //to save license plate location in clipped image
		array<int^, 2>^ tmpPointList = gcnew array<int^, 2>(200000, 10); //to save license plate location on picturebox
		array<int>^ LocNum = gcnew array <int>(200000);//to memory the number of painted points in each image
		array<int>^ ChangedOrNot = gcnew array<int>(200000);//记录这次挑图那些图更改了，更改为1；
		System::String^ directory = "";// directory to save the tmpLogfile to prevent 
		String^ directory_test = "";
									   //System::String^ Lastdirectory = "";// directory to save the tmpLogfile to prevent 
									   //array<String^>^ Loadeddirectory;
									   //array<double,2>^ tmpPointList = gcnew array <double,2>(5,2);
		array<double>^ tmpLPL = gcnew array<double>(10);
		array<String^>^ DirectorySet = gcnew array<String^>(15);//用来存放已经导入过的目录
		int DScount = 0;
		bool DirExist = 0;//目录是否已经读过
	private: System::Windows::Forms::Button^  button9;
	public:
		bool ExamineOrNot = 0;//是否是检验环节，使按键和事件具有不同功能

		//add my functions here 2016
		//function1: Copy the content of System::String to string
		std::string& MarshalString(String ^ s, std::string& os) {
			using namespace Runtime::InteropServices;
			const char* chars =
				(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
			return os;
		}
		
		///function2: 删除指定文件夹下指定文件名的文件  
		/// </summary>  
		/// <param name="url">文件夹地址</param>  
		/// <param name="name">要删除的文件名</param>--自带去除扩展名  
		/// <returns></returns>  
		bool delLicenseFiles(String^ url, String^ name){
			try {
				DirectoryInfo^ Folder = gcnew DirectoryInfo(url);
				for each(FileInfo^ file in Folder->GetFiles()){
					if (name == file->Name->Substring(0, 7)){
						file->Delete();
					}
				}
				//Folder->Delete();
				return true;
			}
			catch (Exception^ e) {
				return false;
			}
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



	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::RichTextBox^  richTextBox3;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::RichTextBox^  richTextBox4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::RichTextBox^  richTextBox5;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog2;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::RichTextBox^  richTextBox2;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::GroupBox^  groupBox5;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::ColumnHeader^  columnHeader4;
	private: System::Windows::Forms::ColumnHeader^  columnHeader5;
	private: System::Windows::Forms::ColumnHeader^  columnHeader6;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
#pragma endregion



#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->richTextBox3 = (gcnew System::Windows::Forms::RichTextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->richTextBox2 = (gcnew System::Windows::Forms::RichTextBox());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->richTextBox4 = (gcnew System::Windows::Forms::RichTextBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->richTextBox5 = (gcnew System::Windows::Forms::RichTextBox());
			this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->tabPage2->SuspendLayout();
			this->tabPage3->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox2
			// 
			this->groupBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->groupBox2->Controls->Add(this->button8);
			this->groupBox2->Controls->Add(this->label2);
			this->groupBox2->Controls->Add(this->richTextBox3);
			this->groupBox2->Controls->Add(this->label1);
			this->groupBox2->Controls->Add(this->richTextBox1);
			this->groupBox2->Controls->Add(this->button2);
			this->groupBox2->Controls->Add(this->button1);
			this->groupBox2->Location = System::Drawing::Point(877, 10);
			this->groupBox2->Margin = System::Windows::Forms::Padding(2);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(2);
			this->groupBox2->Size = System::Drawing::Size(178, 228);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"预处理 / 状态";
			// 
			// button8
			// 
			this->button8->Location = System::Drawing::Point(20, 46);
			this->button8->Margin = System::Windows::Forms::Padding(2);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(136, 40);
			this->button8->TabIndex = 8;
			this->button8->Text = L"从.set导入(检查截图,先点清空)";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &MyForm::button8_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(3, 198);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(73, 20);
			this->label2->TabIndex = 7;
			this->label2->Text = L"已绘点数";
			// 
			// richTextBox3
			// 
			this->richTextBox3->Enabled = false;
			this->richTextBox3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->richTextBox3->Location = System::Drawing::Point(85, 195);
			this->richTextBox3->Margin = System::Windows::Forms::Padding(2);
			this->richTextBox3->Multiline = false;
			this->richTextBox3->Name = L"richTextBox3";
			this->richTextBox3->Size = System::Drawing::Size(73, 26);
			this->richTextBox3->TabIndex = 6;
			this->richTextBox3->Text = L"";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(17, 169);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(57, 20);
			this->label1->TabIndex = 5;
			this->label1->Text = L"当前帧";
			// 
			// richTextBox1
			// 
			this->richTextBox1->Enabled = false;
			this->richTextBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->richTextBox1->Location = System::Drawing::Point(85, 166);
			this->richTextBox1->Margin = System::Windows::Forms::Padding(2);
			this->richTextBox1->Multiline = false;
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(73, 26);
			this->richTextBox1->TabIndex = 2;
			this->richTextBox1->Text = L"";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(20, 90);
			this->button2->Margin = System::Windows::Forms::Padding(2);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(136, 68);
			this->button2->TabIndex = 1;
			this->button2->Text = L"从.set导入(未截过图)";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(20, 16);
			this->button1->Margin = System::Windows::Forms::Padding(2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(137, 26);
			this->button1->TabIndex = 0;
			this->button1->Text = L"清空";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->groupBox3->Controls->Add(this->button5);
			this->groupBox3->Controls->Add(this->button3);
			this->groupBox3->Controls->Add(this->richTextBox2);
			this->groupBox3->Controls->Add(this->button4);
			this->groupBox3->Location = System::Drawing::Point(877, 242);
			this->groupBox3->Margin = System::Windows::Forms::Padding(2);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Padding = System::Windows::Forms::Padding(2);
			this->groupBox3->Size = System::Drawing::Size(177, 134);
			this->groupBox3->TabIndex = 2;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"帧操作";
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(98, 100);
			this->button5->Margin = System::Windows::Forms::Padding(2);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(62, 25);
			this->button5->TabIndex = 8;
			this->button5->Text = L"跳帧";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(21, 16);
			this->button3->Margin = System::Windows::Forms::Padding(2);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(138, 38);
			this->button3->TabIndex = 3;
			this->button3->Text = L"上一帧(W)";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// richTextBox2
			// 
			this->richTextBox2->Location = System::Drawing::Point(21, 100);
			this->richTextBox2->Margin = System::Windows::Forms::Padding(2);
			this->richTextBox2->Multiline = false;
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->Size = System::Drawing::Size(73, 26);
			this->richTextBox2->TabIndex = 7;
			this->richTextBox2->Text = L"";
			this->richTextBox2->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::richTextBox2_KeyDown);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(21, 58);
			this->button4->Margin = System::Windows::Forms::Padding(2);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(138, 38);
			this->button4->TabIndex = 6;
			this->button4->Text = L"下一帧(S)";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// groupBox5
			// 
			this->groupBox5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox5->Controls->Add(this->button9);
			this->groupBox5->Controls->Add(this->button7);
			this->groupBox5->Controls->Add(this->button6);
			this->groupBox5->Location = System::Drawing::Point(877, 380);
			this->groupBox5->Margin = System::Windows::Forms::Padding(2);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Padding = System::Windows::Forms::Padding(2);
			this->groupBox5->Size = System::Drawing::Size(177, 151);
			this->groupBox5->TabIndex = 4;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L" 导出";
			// 
			// button9
			// 
			this->button9->Location = System::Drawing::Point(20, 95);
			this->button9->Margin = System::Windows::Forms::Padding(2);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(136, 47);
			this->button9->TabIndex = 4;
			this->button9->Text = L"导出截取的车牌（列表中所有标过的）";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &MyForm::button9_Click);
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(20, 57);
			this->button7->Margin = System::Windows::Forms::Padding(2);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(136, 34);
			this->button7->TabIndex = 3;
			this->button7->Text = L"导出截取的车牌";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &MyForm::button7_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(20, 16);
			this->button6->Margin = System::Windows::Forms::Padding(2);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(136, 37);
			this->button6->TabIndex = 2;
			this->button6->Text = L"导出目录和坐标到NewFile";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &MyForm::button6_Click);
			// 
			// statusStrip1
			// 
			this->statusStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->statusStrip1->Location = System::Drawing::Point(0, 555);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Padding = System::Windows::Forms::Padding(1, 0, 10, 0);
			this->statusStrip1->Size = System::Drawing::Size(1063, 22);
			this->statusStrip1->TabIndex = 5;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// tabControl1
			// 
			this->tabControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Location = System::Drawing::Point(10, 10);
			this->tabControl1->Margin = System::Windows::Forms::Padding(2);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(863, 523);
			this->tabControl1->TabIndex = 6;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->pictureBox1);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Margin = System::Windows::Forms::Padding(2);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(2);
			this->tabPage1->Size = System::Drawing::Size(855, 497);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"车量图片";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox1->Location = System::Drawing::Point(2, 2);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(2);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(851, 493);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::pictureBox1_Paint);
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBox1_MouseDown);
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->listView1);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Margin = System::Windows::Forms::Padding(2);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(2);
			this->tabPage2->Size = System::Drawing::Size(855, 497);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"图片详情列表";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// listView1
			// 
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(6) {
				this->columnHeader1, this->columnHeader2,
					this->columnHeader3, this->columnHeader4, this->columnHeader5, this->columnHeader6
			});
			this->listView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listView1->Location = System::Drawing::Point(2, 2);
			this->listView1->Margin = System::Windows::Forms::Padding(2);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(851, 493);
			this->listView1->TabIndex = 0;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"帧序号";
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"图片名称";
			this->columnHeader2->Width = 420;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"图片类型";
			this->columnHeader3->Width = 120;
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"图片大小";
			this->columnHeader4->Width = 120;
			// 
			// columnHeader5
			// 
			this->columnHeader5->Text = L"关键点位置(相对原图)";
			this->columnHeader5->Width = 360;
			// 
			// columnHeader6
			// 
			this->columnHeader6->Text = L"文件路径";
			this->columnHeader6->Width = 720;
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->richTextBox4);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Size = System::Drawing::Size(855, 497);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"帮助";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// richTextBox4
			// 
			this->richTextBox4->Dock = System::Windows::Forms::DockStyle::Fill;
			this->richTextBox4->Location = System::Drawing::Point(0, 0);
			this->richTextBox4->Name = L"richTextBox4";
			this->richTextBox4->Size = System::Drawing::Size(855, 497);
			this->richTextBox4->TabIndex = 0;
			this->richTextBox4->Text = resources->GetString(L"richTextBox4.Text");
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->Filter = L"Text files (*.set)|*.set";
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->Filter = L"Text files (*.set)|*.set";
			// 
			// label3
			// 
			this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(6, 535);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(105, 20);
			this->label3->TabIndex = 7;
			this->label3->Text = L"最近标注目录";
			// 
			// richTextBox5
			// 
			this->richTextBox5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->richTextBox5->Location = System::Drawing::Point(129, 535);
			this->richTextBox5->Margin = System::Windows::Forms::Padding(2);
			this->richTextBox5->Multiline = false;
			this->richTextBox5->Name = L"richTextBox5";
			this->richTextBox5->Size = System::Drawing::Size(926, 20);
			this->richTextBox5->TabIndex = 8;
			this->richTextBox5->Text = L"";
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->FileName = L"openFileDialog2";
			this->openFileDialog2->Filter = L"Text files (*.set)|*.set";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(1063, 577);
			this->Controls->Add(this->richTextBox5);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->groupBox5);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->KeyPreview = true;
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"MyForm";
			this->Text = L"LicensePlate+";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResizeEnd += gcnew System::EventHandler(this, &MyForm::MyForm_ResizeEnd);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyDown);
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox5->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->tabPage2->ResumeLayout(false);
			this->tabPage3->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion



	//正常导入.set时执行的操作
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		DirExist = 0;//查重清空
		FormLoad = 0;//置0
		//为了和检查截图不相互干扰，使能一些控件
		button8->Enabled = false; //禁用检查功能
		pictureBox1->Enabled = true;

		//打开.set文件的对话框
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			SegRatioX = 0.1; //ratio that do not use
			SegRatioY = 0.4; //ratio that do not use
			StreamReader^ sr = gcnew StreamReader(openFileDialog1->FileName, System::Text::Encoding::Default);
			String^ Text = sr->ReadLine();
			Text->Replace("\\", "\\\\");//c++中要求双斜杠
			int MaxIndex = Text->Split('\\')->Length;
			//Lastdirectory = directory;
			directory = "";
			for (int index = 0; index < MaxIndex-1; index++) {
				directory += Text->Split('\\')[index] + "\\";
			}
			//directory = Text->Split('.')->Length()
			//richTextBox2->Text = directory;

			if (!File::Exists(Text)) {
				MessageBox::Show("该.set中的图片文件路径不存在！");
				return;
			}

			//Compare with history file
			if (!Directory::Exists("D:\\LicensePlate+_Log")) {
				Directory::CreateDirectory("D:\\LicensePlate+_Log");
			}
			FileStream^ openhistory = gcnew FileStream("D:\\LicensePlate+_Log\\history.log", FileMode::OpenOrCreate, FileAccess::Read);
			StreamReader^ tmpopenhistory = gcnew StreamReader(openhistory, System::Text::Encoding::Unicode);
			String^ histDirectory = tmpopenhistory->ReadLine();
			richTextBox5->Text = histDirectory;//显示上回使用时的目录，方便出错时查找
			String^ IsOver = tmpopenhistory->ReadLine();
			//richTextBox2->Text = IsOver;
			String^ LastImg = tmpopenhistory->ReadLine();//上回相同.set中已标注的图片目录
			String^ LastSelectedRow = tmpopenhistory->ReadLine();//上回相同.set中已标注的图片目录的当前帧
			tmpopenhistory->Close();
			openhistory->Close();
			if (histDirectory != nullptr) {
				DirectorySet[0] = histDirectory;//历史目录也存入目录集
				DScount++;
			}
			for (int iDir = 1; iDir < 15; iDir++){	//查看该目录是否已经导入
				if (DirectorySet[iDir] == directory) {
					DirExist = 1;
				}
			}

			if (IsOver == "0") { //Last .set did not finish when Load a new one
				if (directory != histDirectory) {
					//richTextBox5->Text = histDirectory;
					MessageBox::Show("请导入上次未完成的.set");
					directoryIndicate = 1;
				}
				else if (directory == histDirectory) {
					if (DirExist != 1) {	//该目录之前未导入过
						directoryIndicate = 0;
						DirectorySet[DScount] = directory;
						DScount++;
						if (LastSelectedRow != nullptr) {
							while ((Text = sr->ReadLine()) != LastImg);
							Text = sr->ReadLine();
							SelectedRow += 1;//在MyForm_Load的基础上，把选中的行数调到当前位置；
						}
						//正常导入
						//load
						// Convert System::String to string
						std::string tmpLastImg;
						MarshalString(LastImg, tmpLastImg);
						cvReleaseImage(&img);
						cvReleaseImage(&img_small);
						img = cvLoadImage(tmpLastImg.c_str()); //jump to last location, load img

															   // Clipp img according to segment ratio
						int axis_x = img->width*SegRatioX / 2;
						int axis_y = img->height*SegRatioY;
						int clipp_width = img->width*(1 - SegRatioX);
						int clipp_height = img->height*(1 - SegRatioY);
						cvSetImageROI(img, cvRect(axis_x, axis_y, clipp_width, clipp_height));
						img_small = cvCreateImage(cvSize(clipp_width, clipp_height), img->depth, img->nChannels); //create new image with info of img
						cvCopy(img, img_small, 0);
						cvResetImageROI(img);

						//richTextBox1->Text = (SelectedRow + 1).ToString();
						//toolStripStatusLabel1->Text = "加载中";
						PicCount += 1;
						ListViewItem^ item1 = gcnew ListViewItem(PicCount.ToString(), 0);
						L = Text->Split('\\')->Length;
						item1->SubItems->Add(Text->Split('\\')[L - 1]);
						int tmp_split_len = 0;
						tmp_split_len = Text->Split('.')->Length;
						item1->SubItems->Add(Text->Split('.')[tmp_split_len-1]);
						item1->SubItems->Add(img->width.ToString() + "*" + img->height.ToString());
						item1->SubItems->Add("");
						item1->SubItems->Add(Text);
						listView1->Items->Add(item1);
						try {
							////toolStripStatusLabel1->Text = "加载中";
							do {
								Text = sr->ReadLine();
								PicCount += 1;
								item1 = gcnew ListViewItem(PicCount.ToString(), 0);
								item1->SubItems->Add(Text->Split('\\')[L - 1]);
								int tmp_split_len = 0;
								tmp_split_len = Text->Split('.')->Length;
								item1->SubItems->Add(Text->Split('.')[tmp_split_len-1]);
								//IplImage* tmpImg = cvLoadImage(MarshalString(Text, tmp).c_str());
								item1->SubItems->Add("");
								//cvReleaseImage(&tmpImg);
								item1->SubItems->Add("");
								item1->SubItems->Add(Text);
								listView1->Items->Add(item1);
								//加载已绘点数和tmpPointList以及LocNum

							} while (Text);
							//toolStripStatusLabel1->Text = "加载完成";
						}
						catch (Exception^ e) {
							PicCount -= 1;
							Console::WriteLine("The file could not be read:");
							Console::WriteLine(e->Message);
						}

						//跳转
						SelectedRow = int::Parse(LastSelectedRow);

						// Show img_small in pictureBox1					//并跳转	
						pictureBox1->Image = gcnew System::Drawing::Bitmap(img_small->width, img_small->height, img_small->widthStep,
							System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) img_small->imageData);
						pictureBox1->Refresh();
						richTextBox1->Text = (SelectedRow + 1).ToString();//导入当前帧
					}	//end of if (DirExist != 1)
					else {
						MessageBox::Show("请不要多次打开同一目录！");
					}

				}	//end of directory = histDirectory
			}	//end of if IsOver == "0" 没读完
			else {//已读完
				  //不考虑相同的情况，因为读完的已经导出图片，可重新生成.set再导入
				  //无则直接加载
				if (DirectorySet[0] == directory) {//不能导入上回已经读完的目录
					DirExist = 1;
				}
				if (DirExist != 1) {
					DirectorySet[DScount] = directory;
					DScount++;
					//load
					// Convert System::String to string
					std::string tmp;
					MarshalString(Text, tmp);
					//storage = cvCreateMemStorage(0);
					//if (img) {
					//	delete img;
					//}
					//img = 0;
					//img = nullptr;
					//img_small = nullptr;
					cvReleaseImage(&img);
					cvReleaseImage(&img_small);
					img = cvLoadImage(tmp.c_str()); //load img

													// Clipp img according to segment ratio
					int axis_x = img->width*SegRatioX / 2;
					int axis_y = img->height*SegRatioY;
					int clipp_width = img->width*(1 - SegRatioX);
					int clipp_height = img->height*(1 - SegRatioY);
					cvSetImageROI(img, cvRect(axis_x, axis_y, clipp_width, clipp_height));
					img_small = cvCreateImage(cvSize(clipp_width, clipp_height), img->depth, img->nChannels); //create new image with info of img
					cvCopy(img, img_small, 0);
					cvResetImageROI(img);

					//SelectedRow = PicCount + 1;
					richTextBox1->Text = (SelectedRow + 1).ToString();
					//toolStripStatusLabel1->Text = "加载中";
					PicCount += 1;
					ListViewItem^ item1 = gcnew ListViewItem(PicCount.ToString(), 0);
					L = Text->Split('\\')->Length;
					item1->SubItems->Add(Text->Split('\\')[L - 1]);
					int tmp_split_len = 0;
					tmp_split_len = Text->Split('.')->Length;
					item1->SubItems->Add(Text->Split('.')[tmp_split_len-1]);
					//IplImage* tmpImg = cvLoadImage(MarshalString(Text, tmp).c_str());
					item1->SubItems->Add(img->width.ToString() + "*" + img->height.ToString());
					//cvReleaseImage(&tmpImg);
					item1->SubItems->Add("");
					item1->SubItems->Add(Text);
					listView1->Items->Add(item1);
					try {
						////toolStripStatusLabel1->Text = "加载中";
						do {
							Text = sr->ReadLine();
							PicCount += 1;
							item1 = gcnew ListViewItem(PicCount.ToString(), 0);
							item1->SubItems->Add(Text->Split('\\')[L - 1]);
							int tmp_split_len = 0;
							tmp_split_len = Text->Split('.')->Length;
							item1->SubItems->Add(Text->Split('.')[tmp_split_len-1]);
							//IplImage* tmpImg = cvLoadImage(MarshalString(Text, tmp).c_str());
							item1->SubItems->Add("");
							//cvReleaseImage(&tmpImg);
							item1->SubItems->Add("");
							item1->SubItems->Add(Text);
							listView1->Items->Add(item1);
						} while (Text);
						//toolStripStatusLabel1->Text = "加载完成";
					}
					catch (Exception^ e) {
						PicCount -= 1;
						Console::WriteLine("The file could not be read:");
						Console::WriteLine(e->Message);
					}

					// Show img_small in pictureBox1
					pictureBox1->Image = gcnew System::Drawing::Bitmap(img_small->width, img_small->height, img_small->widthStep,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) img_small->imageData);
					pictureBox1->Refresh();
					//delete img;
					//cvReleaseImage(&img_small);

				}
				else {
					MessageBox::Show("请不要多次打开同一目录！");
				}

			}//end of else if
		}//end of openfile dialog
	}



	//清空键执行的功能（可能在检查图片时有潜在bug，待测试）
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		button6_Click(sender, e); //模拟点击保存NewFile按键
		button7_Click(sender, e); //模拟点击保存图片按钮
		if (pictureBox1->Image != nullptr) {
			button2->Enabled = false;
		}
		if (LocNum[SelectedRow] != 5 && LocNum[SelectedRow] != 0) {
			MessageBox::Show("当前图片标注点数不为0或5时不能清空！");
		}
		else {
			//if (pictureBox1->Image != nullptr)
			//{
			//	FileStream^ cleanhistory = gcnew FileStream("D:\\LicensePlate+_Log\\history.log", FileMode::Create, FileAccess::ReadWrite);
			//	cleanhistory->Close();
			//}
			listView1->Items->Clear();
			PicCount = 0;
			SelectedRow = 0;
			pictureBox1->Image = nullptr;
			richTextBox1->Text = "";
		}
	}



	//上一帧执行的功能
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
		if (ExamineOrNot == 1) {//在检查状态下
			if (SelectedRow == 0) {
				MessageBox::Show("已经到第一帧！");
			}
			else {
				SelectedRow = SelectedRow - 1;
				richTextBox1->Text = (SelectedRow + 1).ToString();
				richTextBox3->Text = "5";

				String^ Text = listView1->Items[SelectedRow]->SubItems[5]->Text;
				Text->Replace("\\", "\\\\");
				// Convert System::String to string
				std::string tmp;
				MarshalString(Text, tmp);
				//storage = cvCreateMemStorage(0);
				//if (img) {
				//	delete img;
				//}
				//img = 0;
				//img = nullptr;
				//img_small = nullptr;
				cvReleaseImage(&img);
				cvReleaseImage(&img_small);
				img = cvLoadImage(tmp.c_str()); //load img
				if (listView1->Items[SelectedRow]->SubItems[3]->Text == "") {
					listView1->Items[SelectedRow]->SubItems[3]->Text = img->width.ToString() + "*" + img->height.ToString();
				}

				// copy img to img_small and Draw 5 points on the img_small
				img_small = cvCreateImage(cvSize(img->width, img->height), img->depth, img->nChannels); //create new image with info of img
				cvCopy(img, img_small, 0);
				cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 0)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 1))), 2, cv::Scalar(0, 0, 255));
				cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 2)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 3))), 2, cv::Scalar(0, 0, 255));
				cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 4)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 5))), 2, cv::Scalar(0, 0, 255));
				cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 6)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 7))), 2, cv::Scalar(0, 0, 255));
				cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 8)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 9))), 2, cv::Scalar(0, 0, 255));

				// Show img_small in pictureBox1
				pictureBox1->Image = gcnew System::Drawing::Bitmap(img_small->width, img_small->height, img_small->widthStep,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) img_small->imageData);
				pictureBox1->Refresh();
				//IplImage **ppImg = &img;
				//delete img;
				//cvReleaseImage(&img_small);
			}

		}
		else {//正常状态而非检查状态
			if (SelectedRow == 0) {
				MessageBox::Show("已经到第一帧！");
			}
			else if (LocNum[SelectedRow]>0 && LocNum[SelectedRow]<5) { //当前图片开始标注但未标注完（标注点数1~4）
				MessageBox::Show("请在 点完5个点/不点任何点 的情况下切换帧！");
			}
			else {
				SelectedRow = SelectedRow - 1;
				richTextBox1->Text = (SelectedRow + 1).ToString();
				richTextBox3->Text = LocNum[SelectedRow].ToString();

				String^ Text = listView1->Items[SelectedRow]->SubItems[5]->Text;
				Text->Replace("\\", "\\\\");
				// Convert System::String to string
				std::string tmp;
				MarshalString(Text, tmp);
				//storage = cvCreateMemStorage(0);
				//if (img) {
				//	delete img;
				//}
				//img = 0;
				//img = nullptr;
				//img_small = nullptr;
				cvReleaseImage(&img);
				cvReleaseImage(&img_small);
				img = cvLoadImage(tmp.c_str()); //load img
				if (listView1->Items[SelectedRow]->SubItems[3]->Text == "") {
					listView1->Items[SelectedRow]->SubItems[3]->Text = img->width.ToString() + "*" + img->height.ToString();
				}
				// Clipp img according to segment ratio
				int axis_x = img->width*SegRatioX / 2;
				int axis_y = img->height*SegRatioY;
				int clipp_width = img->width*(1 - SegRatioX);
				int clipp_height = img->height*(1 - SegRatioY);
				cvSetImageROI(img, cvRect(axis_x, axis_y, clipp_width, clipp_height));
				img_small = cvCreateImage(cvSize(clipp_width, clipp_height), img->depth, img->nChannels); //create new image with info of img
				cvCopy(img, img_small, 0);
				cvResetImageROI(img);
				// Show img_small in pictureBox1
				pictureBox1->Image = gcnew System::Drawing::Bitmap(img_small->width, img_small->height, img_small->widthStep,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) img_small->imageData);
				pictureBox1->Refresh();
				//IplImage **ppImg = &img;
				//delete img;
				//cvReleaseImage(&img_small);
			}
		}//end of 检查状态下
	}



	//下一帧执行的功能
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
		if (ExamineOrNot == 1) {//在检查状态下
			if (SelectedRow == PicCount - 1 || PicCount == 0) {
				MessageBox::Show("已经到最后一帧！");
			}
			else {
				//richTextBox2->Text = (PicCount).ToString();
				//pictureBox1->Load(listView1->Items[SelectedRow]->SubItems[5]->Text);
				SelectedRow = SelectedRow + 1;
				richTextBox1->Text = (SelectedRow + 1).ToString();
				richTextBox3->Text = "5";

				String^ Text = listView1->Items[SelectedRow]->SubItems[5]->Text;
				Text->Replace("\\", "\\\\");
				// Convert System::String to string
				std::string tmp;
				MarshalString(Text, tmp);
				//storage = cvCreateMemStorage(0);
				//if (img) {
				//	delete img;
				//}
				//img = 0;
				//img = nullptr;
				//img_small = nullptr;
				cvReleaseImage(&img);
				cvReleaseImage(&img_small);
				img = cvLoadImage(tmp.c_str()); //load img
												// Update the img size in listview
				if (listView1->Items[SelectedRow]->SubItems[3]->Text == "") {
					listView1->Items[SelectedRow]->SubItems[3]->Text = img->width.ToString() + "*" + img->height.ToString();
				}

				// copy img to img_small and Draw 5 points on the img_small
				img_small = cvCreateImage(cvSize(img->width, img->height), img->depth, img->nChannels); //create new image with info of img
				cvCopy(img, img_small, 0);
				cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 0)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 1))), 2, cv::Scalar(0, 0, 255));
				cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 2)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 3))), 2, cv::Scalar(0, 0, 255));
				cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 4)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 5))), 2, cv::Scalar(0, 0, 255));
				cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 6)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 7))), 2, cv::Scalar(0, 0, 255));
				cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 8)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 9))), 2, cv::Scalar(0, 0, 255));
																																										   
				//// Clipp img according to segment ratio
				//int axis_x = img->width*SegRatioX / 2;
				//int axis_y = img->height*SegRatioY;
				//int clipp_width = img->width*(1 - SegRatioX);
				//int clipp_height = img->height*(1 - SegRatioY);
				//cvSetImageROI(img, cvRect(axis_x, axis_y, clipp_width, clipp_height));
				//img_small = cvCreateImage(cvSize(clipp_width, clipp_height), img->depth, img->nChannels); //create new image with info of img
				//cvCopy(img, img_small, 0);
				//cvResetImageROI(img);
				// Show img_small in pictureBox1
				pictureBox1->Image = gcnew System::Drawing::Bitmap(img_small->width, img_small->height, img_small->widthStep,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) img_small->imageData);
				pictureBox1->Refresh();
				//delete img;
				//cvReleaseImage(&img_small);
			}
		}
		else {//正常状态而非检查状态
			if (SelectedRow == PicCount - 1 || PicCount == 0) {
				MessageBox::Show("已经到最后一帧！");
			}
			else if (LocNum[SelectedRow]>0 && LocNum[SelectedRow]<5) { //当前图片开始标注但未标注完（标注点数1~4）
				MessageBox::Show("请在 点完5个点/不点任何点 的情况下切换帧！");
			}
			else {
				//richTextBox2->Text = (PicCount).ToString();
				//pictureBox1->Load(listView1->Items[SelectedRow]->SubItems[5]->Text);
				SelectedRow = SelectedRow + 1;
				richTextBox1->Text = (SelectedRow + 1).ToString();
				richTextBox3->Text = LocNum[SelectedRow].ToString();

				String^ Text = listView1->Items[SelectedRow]->SubItems[5]->Text;
				Text->Replace("\\", "\\\\");
				// Convert System::String to string
				std::string tmp;
				MarshalString(Text, tmp);
				//storage = cvCreateMemStorage(0);
				//if (img) {
				//	delete img;
				//}
				//img = 0;
				//img = nullptr;
				//img_small = nullptr;
				cvReleaseImage(&img);
				cvReleaseImage(&img_small);
				img = cvLoadImage(tmp.c_str()); //load img
												// Update the img size in listview
				if (listView1->Items[SelectedRow]->SubItems[3]->Text == "") {
					listView1->Items[SelectedRow]->SubItems[3]->Text = img->width.ToString() + "*" + img->height.ToString();
				}
				// Clipp img according to segment ratio
				int axis_x = img->width*SegRatioX / 2;
				int axis_y = img->height*SegRatioY;
				int clipp_width = img->width*(1 - SegRatioX);
				int clipp_height = img->height*(1 - SegRatioY);
				cvSetImageROI(img, cvRect(axis_x, axis_y, clipp_width, clipp_height));
				img_small = cvCreateImage(cvSize(clipp_width, clipp_height), img->depth, img->nChannels); //create new image with info of img
				cvCopy(img, img_small, 0);
				cvResetImageROI(img);
				// Show img_small in pictureBox1
				pictureBox1->Image = gcnew System::Drawing::Bitmap(img_small->width, img_small->height, img_small->widthStep,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) img_small->imageData);
				pictureBox1->Refresh();
				//delete img;
				//cvReleaseImage(&img_small);
			}
		}

	}



	//跳帧键执行的功能
	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
		std::string tmp;
		MarshalString(richTextBox2->Text, tmp);
		double jumpCount = atoi(tmp.c_str());
		if (PicCount == 0) {
			MessageBox::Show("未导入图片！");
		}
		else {
			int kLoad = PicCount - 1;
			while (LocNum[kLoad] == 0 && kLoad != 0) {
				kLoad--;
			}
			int labelCountLoad = kLoad + 1;
			int MaxOf2 = Math::Max(labelCountLoad, SelectedRow);
			if (jumpCount == (int)jumpCount && jumpCount>0 && jumpCount <= MaxOf2) {
				if (LocNum[SelectedRow]>0 && LocNum[SelectedRow]<5) { //当前图片开始标注但未标注完（标注点数1~4）
					MessageBox::Show("请在 点完5个点/不点任何点 的情况下切换帧！");
				}
				else {
					//SelectedRow = int::Parse(richTextBox2->Text);
					SelectedRow = jumpCount - 1;
					richTextBox1->Text = richTextBox2->Text;
					richTextBox3->Text = LocNum[SelectedRow].ToString();
					//pictureBox1->Load(listView1->Items[SelectedRow-1]->SubItems[5]->Text);
					String^ Text = listView1->Items[SelectedRow]->SubItems[5]->Text;
					Text->Replace("\\", "\\\\");
					// Convert System::String to string
					std::string tmp;
					MarshalString(Text, tmp);
					//storage = cvCreateMemStorage(0);
					//if (img) {
					//	delete img;
					//}
					//img = nullptr;
					//img_small = nullptr;
					cvReleaseImage(&img);
					cvReleaseImage(&img_small);
					img = cvLoadImage(tmp.c_str()); //load img
													//update the size
					if (listView1->Items[SelectedRow]->SubItems[3]->Text == "") {
						listView1->Items[SelectedRow]->SubItems[3]->Text = img->width.ToString() + "*" + img->height.ToString();
					}
					// Clipp img according to segment ratio
					int axis_x = img->width*SegRatioX / 2;
					int axis_y = img->height*SegRatioY;
					int clipp_width = img->width*(1 - SegRatioX);
					int clipp_height = img->height*(1 - SegRatioY);
					cvSetImageROI(img, cvRect(axis_x, axis_y, clipp_width, clipp_height));
					img_small = cvCreateImage(cvSize(clipp_width, clipp_height), img->depth, img->nChannels); //create new image with info of img
					cvCopy(img, img_small, 0);
					cvResetImageROI(img);

					// Show img_small in pictureBox1
					pictureBox1->Image = gcnew System::Drawing::Bitmap(img_small->width, img_small->height, img_small->widthStep,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) img_small->imageData);
					pictureBox1->Refresh();
					//delete img;
					//cvReleaseImage(&img_small);
				}
			}
			else if (jumpCount == (int)jumpCount && jumpCount > SelectedRow && jumpCount <= PicCount) {
				MessageBox::Show("不允许跳往后面没看过的图片！请点击下一帧！");
			}
			else {
				MessageBox::Show("输入不合法！请重新输入！");
			}
		}
	}



	//使在跳帧时能够使用Enter（回车）键
	private: System::Void richTextBox2_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		if (e->KeyCode == Keys::Enter) {
			button5_Click(sender, e); //simulate keydown of button 5 跳帧
			button5->Focus();
		}
	}



	//原图坐标和画点相对坐标的2个.log文件另存为.set，且导出标点相对于截图坐标的LPL_small.set
	private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
		int k = PicCount;
		while (LocNum[k] == 0 && k != 0) {
			k--;
		}
		labelCount = k;
		//richTextBox2->Text = labelCount.ToString();
		if (labelCount) {
			//if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			//	//显示文件的全路径
			//	//saveFileDialog1->FileName = DateTime::Now.ToFileTime().ToString();
			//	StreamWriter^ sw = gcnew StreamWriter(saveFileDialog1->FileName);
			//	for (int i = 0; i < labelCount + 1; i++) {
			//		sw->WriteLine(listView1->Items[i]->SubItems[5]->Text + " " + listView1->Items[i]->SubItems[4]->Text);
			//	}
			//	sw->Close();
			//}
			FileStream^ OpenLive = gcnew FileStream("D:\\LicensePlate+_Log\\LiveLog.log", FileMode::OpenOrCreate, FileAccess::ReadWrite);
			StreamReader^ tmpOpenLive = gcnew StreamReader(OpenLive, System::Text::Encoding::Unicode);

			FileStream^ OpenNewFile = gcnew FileStream("D:\\LicensePlate+_Log\\NewFile.set", FileMode::OpenOrCreate, FileAccess::ReadWrite);
			StreamWriter^ tmpOpenNewFile = gcnew StreamWriter(OpenNewFile, System::Text::Encoding::Unicode);
			String^ s;

			while ((s = tmpOpenLive->ReadLine()) != nullptr) {
				tmpOpenNewFile->WriteLine(s);
			}

			tmpOpenLive->Close();
			OpenLive->Close();
			tmpOpenNewFile->Close();
			//OpenNewFile->Close();


			//把tmpPointList里的点存到PicPoint.set里
			FileStream^ PicPoint = gcnew FileStream("D:\\LicensePlate+_Log\\PicPoint.set", FileMode::OpenOrCreate, FileAccess::ReadWrite);
			StreamWriter^ tmpPicPoint = gcnew StreamWriter(PicPoint, System::Text::Encoding::Unicode);
			for (int iPic = 0; iPic <= labelCount;iPic++) {
				//读取坐标
				String^ strPoint = "";
				array<int>^ intPoint = gcnew array<int>(10);
				for (int ipoint = 0; ipoint < 10; ipoint++) {
					if (tmpPointList->GetValue(iPic, ipoint) != nullptr) {//去掉跳过的图片
						intPoint[ipoint] = (int)tmpPointList->GetValue(iPic, ipoint);
						strPoint += intPoint[ipoint].ToString();
						strPoint += ".";
					}
				}
				//写入
				if (listView1->Items[iPic]->SubItems[4]->Text != "") {//若没点坐标就不输入LivePicCount了
					tmpPicPoint->WriteLine(listView1->Items[iPic]->SubItems[5]->Text + " " + strPoint);//画的点的坐标写入LivePicPoint.log
				}
			}
			tmpPicPoint->Close();
			PicPoint->Close();

			//把截取后的坐标存入LPL_small.set
			FileStream^ LPL_clipp = gcnew FileStream("D:\\LicensePlate+_log\\LPL_small.set", FileMode::Create, FileAccess::Write);
			StreamWriter^ tmpLPL_clipp = gcnew StreamWriter(LPL_clipp, System::Text::Encoding::Unicode);
			for (int iImage = 0; iImage <= labelCount; iImage++) {
				if (listView1->Items[iImage]->SubItems[4]->Text != "") {//若没原图点坐标就不计算截图点坐标了
																		//根据点的5个点求车牌的上下左右边界
					int x_left = Math::Min((double)LicensePlateLocation->GetValue(iImage, 0), (double)LicensePlateLocation->GetValue(iImage, 6));
					int x_right = Math::Max((double)LicensePlateLocation->GetValue(iImage, 2), (double)LicensePlateLocation->GetValue(iImage, 4));
					int y_ceil = Math::Min((double)LicensePlateLocation->GetValue(iImage, 1), (double)LicensePlateLocation->GetValue(iImage, 3));
					int y_floor = Math::Max((double)LicensePlateLocation->GetValue(iImage, 5), (double)LicensePlateLocation->GetValue(iImage, 7));

					//按求出的边界扩大（上左右各1.5倍车牌宽，下方一倍车牌宽，不足的话到原图边界即可）
					int LP_width = x_right - x_left;
					int LP_height = y_floor - y_ceil;
					int x_left_ext = Math::Max(x_left - (int)(1.5*LP_width), 0); //最左上点的x坐标
					int y_ceil_ext = Math::Max(y_ceil - (int)(1.5*LP_width), 0); //最左上点的y坐标

																				 //把截图点坐标存入LPL_small以及LPL_small.set
					LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 0) - x_left_ext, iImage, 0);
					LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 1) - y_ceil_ext, iImage, 1);
					LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 2) - x_left_ext, iImage, 2);
					LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 3) - y_ceil_ext, iImage, 3);
					LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 4) - x_left_ext, iImage, 4);
					LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 5) - y_ceil_ext, iImage, 5);
					LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 6) - x_left_ext, iImage, 6);
					LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 7) - y_ceil_ext, iImage, 7);
					LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 8) - x_left_ext, iImage, 8);
					LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 9) - y_ceil_ext, iImage, 9);

					String^ LPL_sm = "";
					LPL_sm += listView1->Items[iImage]->SubItems[5]->Text;//添加图片目录
					LPL_sm += " ";
					for (int p = 0; p < 5; p++) {//添加截图坐标
						LPL_sm += "(" + ((double)LPL_small->GetValue(iImage, 2 * p)).ToString() + "," + ((double)LPL_small->GetValue(iImage, 2 * p + 1)).ToString() + ")" + ".";
					}
					tmpLPL_clipp->WriteLine(LPL_sm);//写入LPL_small.set
				}
			}
			tmpLPL_clipp->Close();
			LPL_clipp->Close();

		}
		else {
			MessageBox::Show("还没有任何坐标信息！");
		}
	}



	//每次绘制pictureBox1时候的操作
	private: System::Void pictureBox1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		if (ExamineOrNot == 1) {}
		else {
			if (pictureBox1->Image != nullptr) {
				//StreamWriter^ tmpsw = gcnew StreamWriter(saveFileDialog1->FileName);
				//Save log in time
				//File::SetAttributes(directory + "\LiveLog.txt", FileAttributes::Normal);
				FileStream^ fs = gcnew FileStream("D:\\LicensePlate+_Log\\LiveLog.log", FileMode::Create, FileAccess::Write);
				int kNum = PicCount;
				while (LocNum[kNum] == 0 && kNum != 0) {
					kNum--;
				}
				int labelCountLive = kNum + 1;
				//richTextBox2->Text = labelCountLive.ToString();
				if (labelCountLive >= 2) {
					//显示文件的全路径
					StreamWriter^ tmpStr = gcnew StreamWriter(fs, System::Text::Encoding::Unicode);

					array<int>^ inttmpPoint = gcnew array<int>(10);
					if (LocNum[SelectedRow] == 5) {
						//Modified on 20161125 start
						int TJPoint[10] = { 0 };
						for (int i = 0; i < 10; i++) {
							TJPoint[i] = (int)tmpPointList->GetValue(SelectedRow, i);
						}
						//判断点有没有标对
						// 横坐标 1>4>0	2>4>3 纵坐标 3>4>0 2>4>1
						if (TJPoint[2]>TJPoint[8] && TJPoint[8]>TJPoint[0] && 
							TJPoint[4]>TJPoint[8] && TJPoint[8]>TJPoint[6] && 
							TJPoint[7]>TJPoint[9] && TJPoint[9]>TJPoint[1] &&
							TJPoint[5]>TJPoint[9] && TJPoint[9]>TJPoint[3]) {
							//若标对
							//清空文件
							FileStream^ fsPoint = gcnew FileStream("D:\\LicensePlate+_log\\LivePicPoint.log", FileMode::Create, FileAccess::Write);
							StreamWriter^ tmpPic = gcnew StreamWriter(fsPoint, System::Text::Encoding::Unicode);
							//写入LivePicCount.log
							for (int i = 0; i < labelCountLive; i++) {
								//读取坐标
								String^ strtmpPoint = "";
								for (int ipoint = 0; ipoint < 10; ipoint++) {
									if (tmpPointList->GetValue(i, ipoint) != nullptr) {//去掉跳过的图片
										inttmpPoint[ipoint] = (int)tmpPointList->GetValue(i, ipoint);
										strtmpPoint += inttmpPoint[ipoint].ToString();
										strtmpPoint += ".";
									}
								}
								//写入
								if (listView1->Items[i]->SubItems[4]->Text != "") {//若没点坐标就不输入LivePicCount了
									tmpPic->WriteLine(listView1->Items[i]->SubItems[5]->Text + " " + strtmpPoint);//画的点的坐标写入LivePicPoint.log
								}
							}
							tmpPic->Close();
							fsPoint->Close();
						}
						else {
							//若没标对
							//删除已经标的5个点
							MessageBox::Show("标点顺序不对！请重新标！");
						}

						//Modified on 20161125 end
					}

					//存入LiveLog
					for (int i = 0; i < labelCountLive; i++) {
						if (listView1->Items[i]->SubItems[4]->Text != "") {//若没点坐标就不输入Livelog了
							tmpStr->WriteLine(listView1->Items[i]->SubItems[5]->Text + " " + listView1->Items[i]->SubItems[4]->Text);
						}
					}
					tmpStr->Close();

				}
				fs->Close();


				//File::SetAttributes(directory + "\LiveLog.txt", FileAttributes::ReadOnly);

				//directory 要实时更新一下
				//directory = ;

				if (directoryIndicate == 0 && labelCountLive != 1) {//仅当状态是读入上次的.set的时候更新


					if (FormLoad == 1) {//是窗体初次加载

					}
					else if (FormLoad == 0) {
						FileStream^ history = gcnew FileStream("D:\\LicensePlate+_Log\\history.log", FileMode::Create, FileAccess::ReadWrite);
						StreamWriter^ tmphistory = gcnew StreamWriter(history, System::Text::Encoding::Unicode);
						tmphistory->WriteLine(directory); //directory without file name
						richTextBox5->Text = directory;
						//if (SelectedRow == PicCount - 1) {   //complete all the images or not
						//	tmphistory->WriteLine("1");
						//}
						//else {
						//	tmphistory->WriteLine("0");
						//}
						//tmphistory->WriteLine(listView1->Items[SelectedRow]->SubItems[5]->Text); //file full directory with file name
						//if(SelectedRow!=0){
						//	tmphistory->WriteLine(SelectedRow); //记录当前帧
						//}
						//else {
						//	tmphistory->WriteLine(SelectedRow+1); //记录当前帧
						//}
						//if (labelCountLive == PicCount && PicCount != 0 && DirectorySet[0] != nullptr) {   //complete all the images or not
						if (labelCountLive == PicCount && PicCount != 0) {   //complete all the images or not
							tmphistory->WriteLine("1");
						}
						else {
							tmphistory->WriteLine("0");
						}
						tmphistory->WriteLine(listView1->Items[labelCountLive - 1]->SubItems[5]->Text); //file full directory with file name
						if (labelCountLive - 1 != 0) {
							tmphistory->WriteLine(labelCountLive - 1); //记录当前帧
						}
						else {
							tmphistory->WriteLine(labelCountLive); //记录当前帧
						}
						tmphistory->Close();
					}
				}

				//refresh the points picturebox
				SolidBrush^ RedBrush = gcnew SolidBrush(Color::Red);	// Create a new pen.
																		//if (ResizeOrNot == 1) { //Form are resized
																		//	////ReCompute the  of license plate location
																		//	//double wfactor1 = (double)img_small->width / pictureBox1->ClientSize.Width;
																		//	//double hfactor1 = (double)img_small->height / pictureBox1->ClientSize.Height;
																		//	//double resizeFactor1 = 1 / Math::Max(wfactor1, hfactor1);
																		//	//double wdiff1 = (pictureBox1->ClientSize.Width - (double)img_small->width*resizeFactor1) / 2;
																		//	//double hdiff1 = (pictureBox1->ClientSize.Height - (double)img_small->height*resizeFactor1) / 2;
																		//	//for (int m = 0; m < LocNum[SelectedRow]; m++) {
																		//	////	e->Graphics->DrawPie(YellowPen, ((int)LicensePlateLocation->GetValue(SelectedRow, 2 * m )- img->width*SegRatioX / 2)*resizeFactor1, (int)LicensePlateLocation->GetValue(SelectedRow, 2 * m+1) *resizeFactor1+hdiff1, 2, 2, 0, 360);
																		//	//	e->Graphics->DrawPie(YellowPen, (200- img->width*SegRatioX / 2)*resizeFactor1, 200 *resizeFactor1+hdiff1, 2, 2, 0, 360);
																		//	//}
																		//	for (int m = 0; m < LocNum[SelectedRow]; m++) {
																		//		e->Graphics->DrawPie(YellowPen, (int)tmpPointList->GetValue(SelectedRow, 2 * m), (int)tmpPointList->GetValue(SelectedRow, 2 * m + 1), 2, 2, 0, 360);
																		//	}
																		//}
																		//else {
				int CircleWidth = 8;
				for (int m = 0; m < LocNum[SelectedRow]; m++) {
					e->Graphics->FillEllipse(RedBrush, (int)tmpPointList->GetValue(SelectedRow, 2 * m) - CircleWidth / 2, (int)tmpPointList->GetValue(SelectedRow, 2 * m + 1) - CircleWidth / 2, CircleWidth, CircleWidth);
				}
				//}
				delete RedBrush;	//Dispose of the pen.
									//OnPaint(e);
									//ResizeOrNot = 0; //reset the indication value
									//richTextBox2->Text = ResizeOrNot.ToString();
			}
		}//end of Examine or not

	}



	//MyForm窗体所定义的快捷键
	private: System::Void MyForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		if (e->KeyCode == Keys::W) {
			button3_Click(sender, e); //simulate keydown of button 3 上一帧
		}
		else if (e->KeyCode == Keys::S) {
			button4_Click(sender, e); //simulate keydown of button 4 下一帧
		}
	}



	//窗口更改大小时刷新pictureBox1的显示（待完善：手画的点对应调整）
	private: System::Void MyForm_ResizeEnd(System::Object^  sender, System::EventArgs^  e) {
		//ResizeOrNot = 1;
		//richTextBox2->Text = ResizeOrNot.ToString();
		pictureBox1->Invalidate();
	}



	//鼠标在pictureBox1上点下后的的操作
	private: System::Void pictureBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		if (pictureBox1->Image) {
			////Draw exits point according to saved file
			//for (i = 0; i < 5; i++) {
			//	if (LicensePlateLocation->GetValue(SelectedRow, i) != 0) {
			//		//draw the point 
			//		richTextBox2->Text = i.ToString();
			//	}
			//	break;
			//}
			//Compute the  of license plate location
			double wfactor = (double)img_small->width / pictureBox1->ClientSize.Width;
			double hfactor = (double)img_small->height / pictureBox1->ClientSize.Height;
			double resizeFactor = 1 / Math::Max(wfactor, hfactor);
			double wdiff = (pictureBox1->ClientSize.Width - (double)img_small->width*resizeFactor) / 2;
			double hdiff = (pictureBox1->ClientSize.Height - (double)img_small->height*resizeFactor) / 2;
			//Convert screen mouse location to pictureBox1 mouse location
			Point^ tmpPoint = pictureBox1->PointToClient(Control::MousePosition);
			if (Math::Max(wfactor, hfactor) == wfactor) {
				if (tmpPoint->Y<hdiff || tmpPoint->Y>(hdiff + (double)img_small->height*resizeFactor)) {
					MessageBox::Show("请在图片高度范围内画点！");
				}
				else {
					if (e->Button == System::Windows::Forms::MouseButtons::Left) {
						//save the point location
						if (LocNum[SelectedRow] < 5) {
							//MessageBox::Show("sfg");
							tmpLocX = tmpPoint->X*wfactor + img->width*SegRatioX / 2;
							tmpLocY = (tmpPoint->Y - hdiff)*wfactor + img->height *SegRatioY;
							LicensePlateLocation->SetValue(tmpLocX, SelectedRow, 2* LocNum[SelectedRow]); //change value of x
							LicensePlateLocation->SetValue(tmpLocY, SelectedRow, 2* LocNum[SelectedRow] +1); //change value of y
							listView1->Items[SelectedRow]->SubItems[4]->Text += "(" + tmpLocX.ToString() + "," + tmpLocY.ToString() + ")" + ".";
							//ControlPaint::DrawReversibleLine(Control::MousePosition, Control::MousePosition, System::Drawing::Color::Red);
							LocNum[SelectedRow]++;
							//draw the point
							tmpPointList->SetValue(tmpPoint->X, SelectedRow, 2 * (LocNum[SelectedRow] - 1));
							tmpPointList->SetValue(tmpPoint->Y, SelectedRow, 2 * (LocNum[SelectedRow] - 1) + 1);
							ChangedOrNot[SelectedRow] = 1;//标注图片改动
							pictureBox1->Invalidate();
						}
						else {
							MessageBox::Show("已点完5个点，如需重绘，请点击右键逐个取消！");
						}
						richTextBox3->Text = LocNum[SelectedRow].ToString();
					}
					else if (e->Button == System::Windows::Forms::MouseButtons::Right) {
						//draw the point
						pictureBox1->Invalidate();
						//save the point location
						if (LocNum[SelectedRow] > 0) {
							LicensePlateLocation->SetValue(0, SelectedRow, 2* (LocNum[SelectedRow] - 1));//change value of x
							LicensePlateLocation->SetValue(0, SelectedRow, 2* (LocNum[SelectedRow] - 1) +1);//change value of y
							String^ tmpText = "";
							for (int j = 0; j < LocNum[SelectedRow] - 1; j++) {
								tmpText += listView1->Items[SelectedRow]->SubItems[4]->Text->Split('.')[j] + ".";
							}
							listView1->Items[SelectedRow]->SubItems[4]->Text = tmpText;
							LocNum[SelectedRow]--;
							ChangedOrNot[SelectedRow] = 1;//标注图片改动
						}
						else {
							MessageBox::Show("已没有点可以取消！");
						}
						richTextBox3->Text = LocNum[SelectedRow].ToString();
					}
				}
			}
			else if (Math::Max(wfactor, hfactor) == hfactor) {
				if (tmpPoint->X<wdiff || tmpPoint->X>(wdiff + (double)img_small->width*resizeFactor)) {
					MessageBox::Show("请在图片宽度范围内画点！");
				}
				else {
					if (e->Button == System::Windows::Forms::MouseButtons::Left) {
						if (LocNum[SelectedRow] < 5) {
							//MessageBox::Show("sfg");
							tmpLocX = (tmpPoint->X- wdiff)*hfactor + img->width*SegRatioX / 2;
							tmpLocY = tmpPoint->Y*hfactor + img->height *SegRatioY;
							LicensePlateLocation->SetValue(tmpLocX, SelectedRow, 2 * LocNum[SelectedRow]); //change value of x
							LicensePlateLocation->SetValue(tmpLocY, SelectedRow, 2 * LocNum[SelectedRow] + 1); //change value of y
							listView1->Items[SelectedRow]->SubItems[4]->Text += "(" + tmpLocX.ToString() + "," + tmpLocY.ToString() + ")" + ".";
							//ControlPaint::DrawReversibleLine(Control::MousePosition, Control::MousePosition, System::Drawing::Color::Red);
							LocNum[SelectedRow]++;
							//draw the point
							tmpPointList->SetValue(tmpPoint->X, SelectedRow, 2 * (LocNum[SelectedRow] - 1));
							tmpPointList->SetValue(tmpPoint->Y, SelectedRow, 2 * (LocNum[SelectedRow] - 1) + 1);
							ChangedOrNot[SelectedRow] = 1;//标注图片改动
							pictureBox1->Invalidate();
						}
						else {
							MessageBox::Show("已点完5个点，如需重绘，请点击右键逐个取消！");
						}
						richTextBox3->Text = LocNum[SelectedRow].ToString();
					}
					else if (e->Button == System::Windows::Forms::MouseButtons::Right) {
						//draw the point
						pictureBox1->Invalidate();
						if (LocNum[SelectedRow] > 0) {
							LicensePlateLocation->SetValue(0, SelectedRow, 2 * (LocNum[SelectedRow]-1));//change value of x
							LicensePlateLocation->SetValue(0, SelectedRow, 2 * (LocNum[SelectedRow] - 1) + 1);//change value of y
							String^ tmpText = "";
							for (int j = 0; j < LocNum[SelectedRow] - 1; j++) {
								tmpText += listView1->Items[SelectedRow]->SubItems[4]->Text->Split('.')[j] + ".";
							}
							listView1->Items[SelectedRow]->SubItems[4]->Text = tmpText;
							LocNum[SelectedRow]--;
							ChangedOrNot[SelectedRow] = 1;//标注图片改动
						}
						else {
							MessageBox::Show("已没有点可以取消！");
						}
						richTextBox3->Text = LocNum[SelectedRow].ToString();
					}
				}
			}
		}
	}



	//导出截取的车牌图片
	private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {
		//Step1: 根据值为0或1的一维数组查看点否更改过
		//Step2：若点更改过，导出到“D:\\LicensePlate+_Log\\PicturesOutput\\”
		IndexShift = 0;//每次导出时复位空图片计数
		for (int iImage = 0; iImage < PicCount; iImage++) {
			if (listView1->Items[iImage]->SubItems[4]->Text == "") {
				IndexShift++; //增加空图片的计数
			}
			if (ChangedOrNot[iImage] == 1 && LocNum[iImage] ==5) {
				//读取车牌图片
				//load
				// Convert System::String to string
				std::string OriImg;
				MarshalString(listView1->Items[iImage]->SubItems[5]->Text, OriImg);
				cvReleaseImage(&img_origin);
				cvReleaseImage(&img_output);//释放图片内存，防止泄露
				img_origin = cvLoadImage(OriImg.c_str()); //load original image

				//根据点的5个点求车牌的上下左右边界
				int x_left = Math::Min((double)LicensePlateLocation->GetValue(iImage, 0), (double)LicensePlateLocation->GetValue(iImage, 6));
				int x_right = Math::Max((double)LicensePlateLocation->GetValue(iImage, 2), (double)LicensePlateLocation->GetValue(iImage, 4));
				int y_ceil = Math::Min((double)LicensePlateLocation->GetValue(iImage, 1), (double)LicensePlateLocation->GetValue(iImage, 3));
				int y_floor = Math::Max((double)LicensePlateLocation->GetValue(iImage, 5), (double)LicensePlateLocation->GetValue(iImage, 7));
				
				//按求出的边界扩大（上左右各1.5倍车牌宽，下方一倍车牌宽，不足的话到原图边界即可）
				int LP_width = x_right - x_left;
				int LP_height = y_floor - y_ceil;
				int x_left_ext = Math::Max(x_left - (int)(1.5*LP_width), 0);
				int x_right_ext = Math::Min(x_right + (int)(1.5*LP_width), (int)(img_origin->width));
				int y_ceil_ext = Math::Max(y_ceil - (int)(1.5*LP_width), 0);
				int y_floor_ext = Math::Min(y_floor + LP_width, (int)(img_origin->height));
				int total_width = x_right_ext - x_left_ext;
				int total_height = y_floor_ext - y_ceil_ext;

				//截取相应图片
				cvSetImageROI(img_origin, cvRect(x_left_ext, y_ceil_ext, total_width, total_height));
				img_output = cvCreateImage(cvSize(total_width, total_height), img_origin->depth, img_origin->nChannels); //create new image with info of img_origin
				cvCopy(img_origin, img_output, 0);
				cvResetImageROI(img_origin);

				//导出到“D:\\LicensePlate+_Log\\PicturesOutput\\”
				std::string outPath = "D:\\LicensePlate+_Log\\PicturesOutput\\";
				String^ outPath_S = "D:\\LicensePlate+_Log\\PicturesOutput\\";
				if (Directory::Exists(outPath_S)==false) {//不存在目录就创建
					Directory::CreateDirectory(outPath_S);
				}


				//把截取后的坐标存入文件名
				//for (int iImage = 0; iImage <= labelCount; iImage++) {
					String^ axis_ten = "";

					if (listView1->Items[iImage]->SubItems[4]->Text != "") {//若没原图点坐标就不计算截图点坐标了
						//文件编号根据IndexShift进行偏移
						axis_ten += (int::Parse(listView1->Items[iImage]->SubItems[0]->Text)- IndexShift).ToString()->PadLeft(7, '0');//编号用0补足7位

						//axis_ten += listView1->Items[iImage]->SubItems[0]->Text->PadLeft(7, '0');//编号用0补足7位
						delLicenseFiles(outPath_S, axis_ten);//删除含相应文件开头的文件
						//根据点的5个点求车牌的上下左右边界
						int x_left = Math::Min((double)LicensePlateLocation->GetValue(iImage, 0), (double)LicensePlateLocation->GetValue(iImage, 6));
						int x_right = Math::Max((double)LicensePlateLocation->GetValue(iImage, 2), (double)LicensePlateLocation->GetValue(iImage, 4));
						int y_ceil = Math::Min((double)LicensePlateLocation->GetValue(iImage, 1), (double)LicensePlateLocation->GetValue(iImage, 3));
						int y_floor = Math::Max((double)LicensePlateLocation->GetValue(iImage, 5), (double)LicensePlateLocation->GetValue(iImage, 7));

						//按求出的边界扩大（上左右各1.5倍车牌宽，下方一倍车牌宽，不足的话到原图边界即可）
						int LP_width = x_right - x_left;
						int LP_height = y_floor - y_ceil;
						int x_left_ext = Math::Max(x_left - (int)(1.5*LP_width), 0); //最左上点的x坐标
						int y_ceil_ext = Math::Max(y_ceil - (int)(1.5*LP_width), 0); //最左上点的y坐标

						//把截图点坐标存入LPL_small以及LPL_small.set
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 0) - x_left_ext, iImage, 0);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 1) - y_ceil_ext, iImage, 1);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 2) - x_left_ext, iImage, 2);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 3) - y_ceil_ext, iImage, 3);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 4) - x_left_ext, iImage, 4);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 5) - y_ceil_ext, iImage, 5);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 6) - x_left_ext, iImage, 6);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 7) - y_ceil_ext, iImage, 7);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 8) - x_left_ext, iImage, 8);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 9) - y_ceil_ext, iImage, 9);

						//axis_ten += listView1->Items[iImage]->SubItems[1]->Text->Split('.')[0];//添加图片目录
						axis_ten += "[";
						for (int p = 0; p < 5; p++) {//添加截图坐标
							axis_ten += "(" + ((double)LPL_small->GetValue(iImage, 2 * p)).ToString() + "," + ((double)LPL_small->GetValue(iImage, 2 * p + 1)).ToString() + ")" + ".";
						}
						axis_ten += "].";
						axis_ten += listView1->Items[iImage]->SubItems[2]->Text;//添加图片类型
					}
					std::string iName;
					MarshalString(axis_ten, iName);
					cvSaveImage((outPath + iName).c_str(), img_output, 0);//存储图片
				}
			}//end of for iImage

	}



	//MyForm窗体关闭时的动作（模拟按下2个导出按键）
	private: System::Void MyForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
		button6_Click(sender, e); //模拟点击保存NewFile按键
		button7_Click(sender, e); //模拟点击保存图片按钮
	}



	//MyForm窗体加载时的动作
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {

		//导入LivePicCount中所有图片，更新tmpPoinList
		if (File::Exists("D:\\LicensePlate+_Log\\PicPoint.set")) {
			FileStream^ fsPicCountLoad = gcnew FileStream("D:\\LicensePlate+_Log\\PicPoint.set", FileMode::Open, FileAccess::Read);
			StreamReader^ srPicCountLoad = gcnew StreamReader(fsPicCountLoad, System::Text::Encoding::Unicode);
			String^ sPicCountLoad;
			//array<int>^ LoadPoint = gcnew array<int>(10);
			int Line = 0;	
			while ((sPicCountLoad = srPicCountLoad->ReadLine()) != nullptr) {
				int strSpaceLength = sPicCountLoad->Split(' ')->Length;
				for (int iLoad = 0; iLoad < 10; iLoad++) {
					tmpPointList->SetValue(int::Parse(sPicCountLoad->Split(' ')[strSpaceLength-1]->Split('.')[iLoad]), Line, iLoad);
				}
				Line++;
			}
			srPicCountLoad->Close();
			fsPicCountLoad->Close();
			richTextBox3->Text = (5).ToString();
		}

		//导入NewFile中所列的图片，并更新变量LocNum，PicCount，SelectedRow,LicensePlateLocation
		if (File::Exists("D:\\LicensePlate+_Log\\NewFile.set")) {
			//Step 1: 导入图片，更新listview
			FileStream^ fsNewFile = gcnew FileStream("D:\\LicensePlate+_Log\\NewFile.set", FileMode::Open, FileAccess::Read);
			StreamReader^ srNewFile = gcnew StreamReader(fsNewFile, System::Text::Encoding::Unicode);
			String^ sNewFile;
			//sNewFile = srNewFile->ReadLine();
			while ((sNewFile = srNewFile->ReadLine()) != nullptr) {//逐行读完文件
				//1.1 显示directory
				int strSpaceLength1 = sNewFile->Split(' ')->Length;
				String^ ExcludeLast = "";
				for (int i = 0; i < strSpaceLength1 - 1; i++) {
					ExcludeLast += sNewFile->Split(' ')[i];
					if (i != strSpaceLength1 - 1) {
						ExcludeLast += " ";
					}
				}
				int MaxIndexNewFile = ExcludeLast->Split('\\')->Length;
				directory = "";
				for (int index = 0; index < MaxIndexNewFile - 1; index++) {
					directory += sNewFile->Split('\\')[index] + "\\";
				}
				richTextBox5->Text = directory;
				//1.2 加载listview1
				richTextBox1->Text = (SelectedRow + 1).ToString();
				//toolStripStatusLabel1->Text = "加载中";
				PicCount += 1;
				ListViewItem^ item1 = gcnew ListViewItem(PicCount.ToString(), 0);
				item1->SubItems->Add(ExcludeLast->Split('\\')[MaxIndexNewFile - 1]); //文件名
				int Ex_split_Len = ExcludeLast->Split('.')->Length; //按长度取最后一个点后面的
				item1->SubItems->Add(ExcludeLast->Split('.')[Ex_split_Len-1]);//文件类型
				item1->SubItems->Add("");//图片大小
				item1->SubItems->Add(sNewFile->Split(' ')[strSpaceLength1-1]);//关键点位置
				item1->SubItems->Add(ExcludeLast);
				listView1->Items->Add(item1);
				//1.3更新参数
				LocNum[SelectedRow] = 5;
				//array<String^>^ LoadLPL_S = gcnew array<String^>(5);
				array<int>^ LoadLPL = gcnew array<int>(10);
				for (int iLPL = 0; iLPL < 5; iLPL++) {
					LoadLPL[2*iLPL] = int::Parse(sNewFile->Split(' ')[strSpaceLength1-1]->Split('.')[iLPL]->Split('(')[1]->Split(')')[0]->Split(',')[0]);//截取并去括号
					LoadLPL[2 * iLPL+1] = int::Parse(sNewFile->Split(' ')[strSpaceLength1 - 1]->Split('.')[iLPL]->Split('(')[1]->Split(')')[0]->Split(',')[1]);//截取并去括号
				}

				for (int iLPL = 0; iLPL < 10; iLPL++) {
					LicensePlateLocation->SetValue(LoadLPL[iLPL], SelectedRow, iLPL);
				}
				SelectedRow += 1;
				//tmpPointList->SetValue(SelectedRow,)
			}
			//richTextBox3->Text = SelectedRow.ToString();
			//richTextBox2->Text = PicCount.ToString();

			// Convert System::String to string
			std::string tmpsNewFile;
			String^ FinalNewFile = listView1->Items[SelectedRow - 1]->SubItems[5]->Text;
			MarshalString(FinalNewFile, tmpsNewFile);
			cvReleaseImage(&img);
			cvReleaseImage(&img_small);
			img = cvLoadImage(tmpsNewFile.c_str()); //load img

													// Clipp img according to segment ratio
			int axis_x = img->width*SegRatioX / 2;
			int axis_y = img->height*SegRatioY;
			int clipp_width = img->width*(1 - SegRatioX);
			int clipp_height = img->height*(1 - SegRatioY);
			cvSetImageROI(img, cvRect(axis_x, axis_y, clipp_width, clipp_height));
			img_small = cvCreateImage(cvSize(clipp_width, clipp_height), img->depth, img->nChannels); //create new image with info of img
			cvCopy(img, img_small, 0);
			cvResetImageROI(img);

			FormLoad = 1;//指明是窗体初次加载

			// Show img_small in pictureBox1
			pictureBox1->Image = gcnew System::Drawing::Bitmap(img_small->width, img_small->height, img_small->widthStep,
				System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) img_small->imageData);
			pictureBox1->Refresh();

			SelectedRow -= 1;

			srNewFile->Close();
			fsNewFile->Close();
		}
	}



	//导入截图以供检查功能(待完善)
	private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) {//从.set导入（截过的图）
		if (pictureBox1->Image != nullptr) {
			MessageBox::Show("请先点击“清空”！");
		}
		else {
			ExamineOrNot = 1; //显示启用了截图功能
			SegRatioX = 0.0; //ratio that do not use
			SegRatioY = 0.0; //ratio that do not use
			pictureBox1->Enabled = false;
			button2->Enabled = false; //禁用导入功能
			button6->Enabled = false; //禁用导出文件
			button7->Enabled = false; //禁用导出图片
			if (openFileDialog2->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				StreamReader^ srTest = gcnew StreamReader(openFileDialog2->FileName, System::Text::Encoding::Default);
				String^ Text_test = srTest->ReadLine();
				Text_test->Replace("\\", "\\\\");
				int MaxIndex_test = Text_test->Split('\\')->Length;
				String^ directory_test = "";
				for (int index = 0; index < MaxIndex_test - 1; index++) {
					directory_test += Text_test->Split('\\')[index] + "\\";
				}
				//directory = Text->Split('.')->Length()
				//richTextBox2->Text = directory;
				if (directory_test != "D:\\LicensePlate+_Log\\PicturesOutput\\") {
					MessageBox::Show("只支持D:\LicensePlate + _Log\PicturesOutput(即导出目录)下生成的dir.set文件");
					return;
				}
				else {
					if (!File::Exists(Text_test)) {
						MessageBox::Show("该.set中的图片文件路径不存在！");
						return;
					}

					//正常导入
					// Convert System::String to string
					std::string tmpTestImg;
					MarshalString(Text_test, tmpTestImg);
					cvReleaseImage(&img);
					cvReleaseImage(&img_small);
					img_small = cvLoadImage(tmpTestImg.c_str()); //readline,load img

																 //richTextBox1->Text = (SelectedRow + 1).ToString();
																 //toolStripStatusLabel1->Text = "加载中";
					PicCount += 1;
					ListViewItem^ item1 = gcnew ListViewItem(PicCount.ToString(), 0);
					L = Text_test->Split('\\')->Length;
					item1->SubItems->Add(Text_test->Split('\\')[L - 1]);
					item1->SubItems->Add(Text_test->Split(']')[1]->Split('.')[1]);
					item1->SubItems->Add(img_small->width.ToString() + "*" + img_small->height.ToString());
					item1->SubItems->Add(Text_test->Split(']')[0]->Split('[')[1]);
					item1->SubItems->Add(Text_test);
					listView1->Items->Add(item1);

					//更新LicensePlateLocation
					String^ testLPL = "";
					int L_test = Text_test->Split('\\')->Length;
					testLPL = Text_test->Split('\\')[L_test - 1]->Split('[')[1];
					String^a = testLPL->Split('.')[0]->Split(',')[1]->Split(')')[0];
					LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[0]->Split(',')[0]->Split('(')[1]), PicCount - 1, 0);
					LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[0]->Split(',')[1]->Split(')')[0]), PicCount - 1, 1);
					LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[1]->Split(',')[0]->Split('(')[1]), PicCount - 1, 2);
					LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[1]->Split(',')[1]->Split(')')[0]), PicCount - 1, 3);
					LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[2]->Split(',')[0]->Split('(')[1]), PicCount - 1, 4);
					LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[2]->Split(',')[1]->Split(')')[0]), PicCount - 1, 5);
					LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[3]->Split(',')[0]->Split('(')[1]), PicCount - 1, 6);
					LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[3]->Split(',')[1]->Split(')')[0]), PicCount - 1, 7);
					LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[4]->Split(',')[0]->Split('(')[1]), PicCount - 1, 8);
					LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[4]->Split(',')[1]->Split(')')[0]), PicCount - 1, 9);

					try {
						do {
							Text_test = srTest->ReadLine();
							PicCount += 1;
							item1 = gcnew ListViewItem(PicCount.ToString(), 0);
							item1->SubItems->Add(Text_test->Split('\\')[L - 1]);
							item1->SubItems->Add(Text_test->Split(']')[1]->Split('.')[1]);
							item1->SubItems->Add("");
							item1->SubItems->Add(Text_test->Split(']')[0]->Split('[')[1]);
							item1->SubItems->Add(Text_test);
							listView1->Items->Add(item1);
							//加载已绘点数和tmpPointList以及LocNum

							//更新LicensePlateLocation
							String^ testLPL = "";
							int L_test = Text_test->Split('\\')->Length;
							testLPL = Text_test->Split('\\')[L_test - 1]->Split('[')[1];
							LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[0]->Split(',')[0]->Split('(')[1]), PicCount - 1, 0);
							LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[0]->Split(',')[1]->Split(')')[0]), PicCount - 1, 1);
							LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[1]->Split(',')[0]->Split('(')[1]), PicCount - 1, 2);
							LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[1]->Split(',')[1]->Split(')')[0]), PicCount - 1, 3);
							LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[2]->Split(',')[0]->Split('(')[1]), PicCount - 1, 4);
							LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[2]->Split(',')[1]->Split(')')[0]), PicCount - 1, 5);
							LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[3]->Split(',')[0]->Split('(')[1]), PicCount - 1, 6);
							LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[3]->Split(',')[1]->Split(')')[0]), PicCount - 1, 7);
							LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[4]->Split(',')[0]->Split('(')[1]), PicCount - 1, 8);
							LicensePlateLocation->SetValue(double::Parse(testLPL->Split('.')[4]->Split(',')[1]->Split(')')[0]), PicCount - 1, 9);

							//tmpPointList->SetValue(int::Parse(testLPL->Split('.')[0]->Split(',')[0]->Split('(')[1])/wfactor + wdiff, PicCount - 1, 0);
							//tmpPointList->SetValue(int::Parse(testLPL->Split('.')[0]->Split(',')[1]->Split(')')[0])/wfactor, PicCount - 1, 1);
							//tmpPointList->SetValue(int::Parse(testLPL->Split('.')[1]->Split(',')[0]->Split('(')[1])/wfactor + wdiff, PicCount - 1, 2);
							//tmpPointList->SetValue(int::Parse(testLPL->Split('.')[1]->Split(',')[1]->Split(')')[0])/wfactor, PicCount - 1, 3);
							//tmpPointList->SetValue(int::Parse(testLPL->Split('.')[2]->Split(',')[0]->Split('(')[1])/wfactor + wdiff, PicCount - 1, 4);
							//tmpPointList->SetValue(int::Parse(testLPL->Split('.')[2]->Split(',')[1]->Split(')')[0])/wfactor, PicCount - 1, 5);
							//tmpPointList->SetValue(int::Parse(testLPL->Split('.')[3]->Split(',')[0]->Split('(')[1])/wfactor + wdiff, PicCount - 1, 6);
							//tmpPointList->SetValue(int::Parse(testLPL->Split('.')[3]->Split(',')[1]->Split(')')[0])/wfactor, PicCount - 1, 7);
							//tmpPointList->SetValue(int::Parse(testLPL->Split('.')[4]->Split(',')[0]->Split('(')[1])/wfactor + wdiff, PicCount - 1, 8);
							//tmpPointList->SetValue(int::Parse(testLPL->Split('.')[4]->Split(',')[1]->Split(')')[0])/wfactor, PicCount - 1, 9);

						} while (Text_test);
						//toolStripStatusLabel1->Text = "加载完成";
					}
					catch (Exception^ e) {
						PicCount -= 1;
						Console::WriteLine("The file could not be read:");
						Console::WriteLine(e->Message);
					}

					// copy img to img_small and Draw 5 points on the img_small
					cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 0)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 1))), 2, cv::Scalar(0, 0, 255));
					cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 2)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 3))), 2, cv::Scalar(0, 0, 255));
					cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 4)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 5))), 2, cv::Scalar(0, 0, 255));
					cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 6)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 7))), 2, cv::Scalar(0, 0, 255));
					cvCircle(img_small, CvPoint((int)(double)(LicensePlateLocation->GetValue(SelectedRow, 8)), (int)(double)(LicensePlateLocation->GetValue(SelectedRow, 9))), 2, cv::Scalar(0, 0, 255));

					// Show img_small in pictureBox1					//并跳转	
					pictureBox1->Image = gcnew System::Drawing::Bitmap(img_small->width, img_small->height, img_small->widthStep,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) img_small->imageData);
					pictureBox1->Refresh();
					richTextBox1->Text = (SelectedRow + 1).ToString();//导入当前帧
				}//end of directory justify

			}//end of openfiledialog2
		}
	}



	//图片全部导出
	private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e) {
	//Messagebox提示用户是否全部保存
	System::Windows::Forms::DialogResult result_m = MessageBox::Show("确定要全部保存吗？\n（会清空D:\\LicensePlate+_Log\\ALLOutput下图片并保存所有图片详情列表中标过的图）", "提示：", MessageBoxButtons::OKCancel, MessageBoxIcon::Question);
	if (result_m == System::Windows::Forms::DialogResult::OK) {
		//Step1: 根据值为0或1的一维数组查看点否更改过
		//Step2：若点更改过，导出到“D:\\LicensePlate+_Log\\PicturesOutput\\”
		IndexShift = 0;//每次导出时复位空图片计数
		for (int iImage = 0; iImage < PicCount; iImage++) {
			if (listView1->Items[iImage]->SubItems[4]->Text == "") {
				IndexShift++; //增加空图片的计数
			} 
				if (LocNum[iImage] == 5) {
					//读取车牌图片
					//load
					// Convert System::String to string
					std::string OriImg;
					MarshalString(listView1->Items[iImage]->SubItems[5]->Text, OriImg);
					cvReleaseImage(&img_origin);
					cvReleaseImage(&img_output);//释放图片内存，防止泄露
					img_origin = cvLoadImage(OriImg.c_str()); //load original image

															  //根据点的5个点求车牌的上下左右边界
					int x_left = Math::Min((double)LicensePlateLocation->GetValue(iImage, 0), (double)LicensePlateLocation->GetValue(iImage, 6));
					int x_right = Math::Max((double)LicensePlateLocation->GetValue(iImage, 2), (double)LicensePlateLocation->GetValue(iImage, 4));
					int y_ceil = Math::Min((double)LicensePlateLocation->GetValue(iImage, 1), (double)LicensePlateLocation->GetValue(iImage, 3));
					int y_floor = Math::Max((double)LicensePlateLocation->GetValue(iImage, 5), (double)LicensePlateLocation->GetValue(iImage, 7));

					//按求出的边界扩大（上左右各1.5倍车牌宽，下方一倍车牌宽，不足的话到原图边界即可）
					int LP_width = x_right - x_left;
					int LP_height = y_floor - y_ceil;
					int x_left_ext = Math::Max(x_left - (int)(1.5*LP_width), 0);
					int x_right_ext = Math::Min(x_right + (int)(1.5*LP_width), (int)(img_origin->width));
					int y_ceil_ext = Math::Max(y_ceil - (int)(1.5*LP_width), 0);
					int y_floor_ext = Math::Min(y_floor + LP_width, (int)(img_origin->height));
					int total_width = x_right_ext - x_left_ext;
					int total_height = y_floor_ext - y_ceil_ext;

					//截取相应图片
					cvSetImageROI(img_origin, cvRect(x_left_ext, y_ceil_ext, total_width, total_height));
					img_output = cvCreateImage(cvSize(total_width, total_height), img_origin->depth, img_origin->nChannels); //create new image with info of img_origin
					cvCopy(img_origin, img_output, 0);
					cvResetImageROI(img_origin);

					//导出到“D:\\LicensePlate+_Log\\PicturesOutput\\”
					std::string outPath = "D:\\LicensePlate+_Log\\ALLOutput\\";
					String^ outPath_S = "D:\\LicensePlate+_Log\\ALLOutput\\";
					if (Directory::Exists(outPath_S) == false) {//不存在目录就创建
						Directory::CreateDirectory(outPath_S);
					}


					//把截取后的坐标存入文件名
					//for (int iImage = 0; iImage <= labelCount; iImage++) {
					String^ axis_ten = "";

					if (listView1->Items[iImage]->SubItems[4]->Text != "") {//若没原图点坐标就不计算截图点坐标了
																			//文件编号根据IndexShift进行偏移
						axis_ten += (int::Parse(listView1->Items[iImage]->SubItems[0]->Text) - IndexShift).ToString()->PadLeft(7, '0');//编号用0补足7位

																																	   //axis_ten += listView1->Items[iImage]->SubItems[0]->Text->PadLeft(7, '0');//编号用0补足7位
						delLicenseFiles(outPath_S, axis_ten);//删除含相应文件开头的文件
															 //根据点的5个点求车牌的上下左右边界
						int x_left = Math::Min((double)LicensePlateLocation->GetValue(iImage, 0), (double)LicensePlateLocation->GetValue(iImage, 6));
						int x_right = Math::Max((double)LicensePlateLocation->GetValue(iImage, 2), (double)LicensePlateLocation->GetValue(iImage, 4));
						int y_ceil = Math::Min((double)LicensePlateLocation->GetValue(iImage, 1), (double)LicensePlateLocation->GetValue(iImage, 3));
						int y_floor = Math::Max((double)LicensePlateLocation->GetValue(iImage, 5), (double)LicensePlateLocation->GetValue(iImage, 7));

						//按求出的边界扩大（上左右各1.5倍车牌宽，下方一倍车牌宽，不足的话到原图边界即可）
						int LP_width = x_right - x_left;
						int LP_height = y_floor - y_ceil;
						int x_left_ext = Math::Max(x_left - (int)(1.5*LP_width), 0); //最左上点的x坐标
						int y_ceil_ext = Math::Max(y_ceil - (int)(1.5*LP_width), 0); //最左上点的y坐标

																					 //把截图点坐标存入LPL_small以及LPL_small.set
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 0) - x_left_ext, iImage, 0);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 1) - y_ceil_ext, iImage, 1);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 2) - x_left_ext, iImage, 2);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 3) - y_ceil_ext, iImage, 3);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 4) - x_left_ext, iImage, 4);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 5) - y_ceil_ext, iImage, 5);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 6) - x_left_ext, iImage, 6);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 7) - y_ceil_ext, iImage, 7);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 8) - x_left_ext, iImage, 8);
						LPL_small->SetValue((double)LicensePlateLocation->GetValue(iImage, 9) - y_ceil_ext, iImage, 9);

						//axis_ten += listView1->Items[iImage]->SubItems[1]->Text->Split('.')[0];//添加图片目录
						axis_ten += "[";
						for (int p = 0; p < 5; p++) {//添加截图坐标
							axis_ten += "(" + ((double)LPL_small->GetValue(iImage, 2 * p)).ToString() + "," + ((double)LPL_small->GetValue(iImage, 2 * p + 1)).ToString() + ")" + ".";
						}
						axis_ten += "].";
						axis_ten += listView1->Items[iImage]->SubItems[2]->Text;//添加图片类型
					}
					std::string iName;
					MarshalString(axis_ten, iName);
					cvSaveImage((outPath + iName).c_str(), img_output, 0);//存储图片
				}
		}//end of for iImage
	}//end of messageboxOKCancel

}
};
}
