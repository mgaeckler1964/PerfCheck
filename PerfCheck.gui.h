/*
	this file was created from C:\CRESD\Source\PerfCheck\PerfCheck.gui with MG Gui Builder
	+++++++++++++++++++++++
	!!!!! Do not Edit !!!!!
	+++++++++++++++++++++++
*/
#ifndef C__CRESD_SOURCE_PERFCHECK_PERFCHECK_GUI_H
#define C__CRESD_SOURCE_PERFCHECK_PERFCHECK_GUI_H
#include <winlib/popup.h>
#include <winlib/frame.h>
#include <winlib/scrollFrame.h>
#include <winlib/ControlW.h>
#include <winlib/xmlEditorChild.h>
#include <winlib/chartWin.h>
#include <winlib/gridView.h>

#include <winlib/winApp.h>

namespace winlibGUI {

	const int CHARTCHILD_id=133;
	const int CheckFORM_id=128;
	const int ControlCHILD_id=132;
	const int CountPUSHBUTTON_id=130;
	const int FunctionNameBOX_id=131;
	const int TimePUSHBUTTON_id=129;

	class GuiApplication : public winlib::Application {
		public:
		virtual gak::xml::Document *getGuiDoc();
		GuiApplication(int iconID=-1) : winlib::Application(iconID) {}
	};

	class CheckFORM_form : public winlib::OverlappedWindow {
		public:
		CheckFORM_form(winlib::BasicWindow *owner) : OverlappedWindow(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return OverlappedWindow::create(parent,"CheckFORM");
		}

		winlib::PushButton *TimePUSHBUTTON;
		winlib::PushButton *CountPUSHBUTTON;
		winlib::ListBox *FunctionNameBOX;
		winlib::FrameChild *ControlCHILD;
		winlib::ChartChild *CHARTCHILD;
		private:
		virtual void getControls();
	};	// end of CheckFORM
}	// namespace winlibGUI

#endif // C__CRESD_SOURCE_PERFCHECK_PERFCHECK_GUI_H
