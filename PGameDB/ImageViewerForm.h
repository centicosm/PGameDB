/*
Copyright (C) 2009 Centicosm Software


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


// Form used to view and resize images

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PGameDB {

  public ref class ImageViewerForm : public System::Windows::Forms::Form
  {
  public:
    ImageViewerForm(String ^img) {
      InitializeComponent();

      // the image will be in the shot path
      if (!System::IO::File::Exists(img)) {
        const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
        if (!datapath) {
          MessageBox::Show("Error: Configuration file corrupted!");
          return;
        }

        img = gcnew String(datapath);
        img = img->Replace("/", "\\");
        img = img->Replace(".", "");
        img = String::Concat(Environment::CurrentDirectory, img);
        img = String::Concat(img, "invalid.jpg");
      }
      ImageViewerImage->Load(img);
      Width = ImageViewerImage->Image->Width;
      Height = ImageViewerImage->Image->Height;

      // track the aspect ratio so we can maintain it during image resizing
      AspectRatio = (float)this->Width / (float)this->Height;
      ImageViewerImage->Refresh();

    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~ImageViewerForm()
    {
      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::PictureBox^  ImageViewerImage;
  protected: 

  protected: 

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>
    System::ComponentModel::Container ^components;

    float AspectRatio;

#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      this->ImageViewerImage = (gcnew System::Windows::Forms::PictureBox());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImageViewerImage))->BeginInit();
      this->SuspendLayout();
      // 
      // ImageViewerImage
      // 
      this->ImageViewerImage->Dock = System::Windows::Forms::DockStyle::Fill;
      this->ImageViewerImage->Location = System::Drawing::Point(0, 0);
      this->ImageViewerImage->Name = L"ImageViewerImage";
      this->ImageViewerImage->Size = System::Drawing::Size(284, 264);
      this->ImageViewerImage->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
      this->ImageViewerImage->TabIndex = 0;
      this->ImageViewerImage->TabStop = false;
      // 
      // ImageViewerForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(284, 264);
      this->Controls->Add(this->ImageViewerImage);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"ImageViewerForm";
      this->ShowIcon = false;
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
      this->Text = L"View Image";
      this->ResizeEnd += gcnew System::EventHandler(this, &ImageViewerForm::ImageViewerForm_ResizeEnd);
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImageViewerImage))->EndInit();
      this->ResumeLayout(false);

    }
#pragma endregion

  private: 

    // make sure the image stays at the correct aspect ratio
    System::Void ImageViewerForm_ResizeEnd(System::Object^  sender, System::EventArgs^  e) {
      this->Height = (int)((float)this->Width / AspectRatio);
    }
  };
}
