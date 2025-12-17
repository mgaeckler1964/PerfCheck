/*
	this file was created from C:\CRESD\Source\PerfCheck\PerfCheck.gui with MG Gui Builder
	+++++++++++++++++++++++
	!!!!! Do not Edit !!!!!
	+++++++++++++++++++++++
*/

#include "C:\CRESD\Source\PerfCheck\PerfCheck.gui.h"

namespace winlibGUI {

	gak::xml::Document *GuiApplication::getGuiDoc() {
		gak::xml::Document *doc = winlib::Application::getGuiDoc();
		if(!doc) {
			gak::STRING xmlSrc = "<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>\n"
"<gui>\n"
"<forms>\n"
"<form name=\"CheckFORM\" style=\"277807104\" width=\"758\" height=\"491\" caption=\"Performance Check\" id=\"128\" baseClass=\"OverlappedWindow\" bgColor=\"Select\" bg_red=\"151\" bg_green=\"255\" bg_blue=\"221\">\n"
"<layoutManager type=\"AttachmentManager\" />\n"
"<child type=\"FrameChild\" name=\"ControlCHILD\" caption=\"\" x=\"0\" y=\"0\" width=\"280\" height=\"456\" style=\"1342308352\" id=\"132\" bgColor=\"NULL_BRUSH\" bg_red=\"0\" bg_green=\"0\" bg_blue=\"0\">\n"
"<layout attachment=\"0\" growHeight=\"2\" x=\"0\" y=\"0\" />\n"
"<child type=\"PushButton\" name=\"TimePUSHBUTTON\" caption=\"Time\" x=\"8\" y=\"8\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"129\" />\n"
"<child type=\"PushButton\" name=\"CountPUSHBUTTON\" caption=\"Count\" x=\"96\" y=\"8\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"130\" />\n"
"<child type=\"ListBox\" name=\"FunctionNameBOX\" caption=\"\" x=\"8\" y=\"40\" width=\"256\" height=\"392\" style=\"1353777409\" id=\"131\" bgColor=\"Select\" bg_red=\"240\" bg_green=\"230\" bg_blue=\"140\" />\n"
"</child>\n"
"<child type=\"ChartChild\" name=\"CHARTCHILD\" caption=\"\" x=\"407\" y=\"145\" width=\"80\" height=\"25\" style=\"1342177280\" id=\"133\">\n"
"<layout attachment=\"4\" growWidth=\"1\" growHeight=\"1\" />\n"
"</child>\n"
"</form>\n"
"</forms>\n"
"</gui>";
			return Application::getGuiDoc( xmlSrc );
		}
		return doc;
	}


	void CheckFORM_form::getControls() {
		TimePUSHBUTTON=static_cast<winlib::PushButton*>(findChild(TimePUSHBUTTON_id));
		CountPUSHBUTTON=static_cast<winlib::PushButton*>(findChild(CountPUSHBUTTON_id));
		FunctionNameBOX=static_cast<winlib::ListBox*>(findChild(FunctionNameBOX_id));
		ControlCHILD=static_cast<winlib::FrameChild*>(findChild(ControlCHILD_id));
		CHARTCHILD=static_cast<winlib::ChartChild*>(findChild(CHARTCHILD_id));
	}	// CheckFORM
}	// namespace winlibGUI
