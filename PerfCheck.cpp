/*
		Project:		PerfCheck
		Module:			PerfCheck.cpp
		Description:	Perfomance Check, analyse timings.csv from gaklib
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <fstream>

#include <gak/numericString.h>
#include <gak/fmtNumber.h>
#include <gak/shared.h>
#include <gak/exception.h>
#include <gak/csv.h>

#include <WINLIB/WINAPP.H>
#include <WINLIB/DEVICE.H>

#include "PerfCheck_rc.h"
#include "PerfCheck.gui.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

using namespace winlib;
using namespace winlibGUI;

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

static const size_t MIN_COUNT = 3;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class CheckMainWindow : public CheckFORM_form
{
	typedef gak::PairMap<gak::STRING, Chart2D>	TimingsData;
	typedef gak::PairMap<gak::STRING, Chart1D>	TotalData;

	STRING		m_cmdLine;
	TimingsData	m_timingsData;
	TotalData	m_totalTime, m_totalCount;

	virtual ProcessStatus handleCreate();
	virtual ProcessStatus handleSelectionChange( int control );
	virtual ProcessStatus handleButtonClick( int control );
public:
	CheckMainWindow();
	SuccessCode create(const STRING &cmdLine);
};

class WindowsApplication : public GuiApplication
{
	virtual bool startApplication( HINSTANCE /*hInstance*/, const char *cmdLine )
	{
		if( !cmdLine || !*cmdLine )
			throw gak::LibraryException("Bad command line. Missing timings.csv!");
		doEnableLogEx(gakLogging::llInfo);
		doDisableLog();
		setApplication("PerfCheck");
		setComapny("gak");
		return false;
	}
	virtual CallbackWindow  *createMainWindow( const char *cmdLine, int /*nCmdShow*/ )
	{
		std::auto_ptr<CheckMainWindow>	mainWindow( new CheckMainWindow );
		if( mainWindow->create( cmdLine ) == scERROR )
		{
			throw gak::LibraryException( "Could not create window!" );
		}
		mainWindow->focus();

		return mainWindow.release();
	}
	virtual void deleteMainWindow( BasicWindow  *mainWindow )
	{
		delete mainWindow;
	}

	public:
	WindowsApplication() : GuiApplication( IDI_PERF_CHECK ) {}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static WindowsApplication	app;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static COLORREF GetNextColor()
{
	static const COLORREF color[] =
	{
		colors::RED,
		colors::LIME,
		colors::BLUE,
		colors::YELLOW,
		colors::MAGENTA,
		colors::CYAN,
		colors::MAROON,
		colors::GRAY,
		colors::GREEN,
		colors::NAVY,
		colors::OLIVE,
		colors::PURPE,
		colors::TEAL,
		colors::DARK_RED,
		colors::BROWN,
		colors::ORANGE,
		colors::SILVER,
		colors::KHAKI,
		colors::YELLOW_GREEN,
		colors::TURQUOISE,
		colors::AQUA_MARINE,
		colors::BLUE_VIOLET,
		colors::INDIGO,
		colors::BLACK
	};
	static const size_t s_numColors = arraySize(color);
	static size_t s_curColor = size_t(-1);

	++s_curColor;
	if( s_curColor >= s_numColors )
		s_curColor = 0;
	return color[s_curColor];
}

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

CheckMainWindow::CheckMainWindow() : CheckFORM_form( nullptr )
{
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

winlib::SuccessCode CheckMainWindow::create(const STRING &cmdLine)
{
	m_cmdLine = cmdLine;
	if( !gak::exists(m_cmdLine) )
	{
		throw gak::LibraryException( m_cmdLine + " does not exist!");
	}
	gak::ArrayOfStrings	csvLine;
	std::ifstream	inp( m_cmdLine );
	while( !inp.eof() )
	{
		readCSVLine(inp, &csvLine, ',' );
		if( csvLine.size() >= 8 && csvLine[0] != "file" )
		{
			size_t count = csvLine[7].getValueN<double>();
			if( count > MIN_COUNT )
			{
				m_totalCount[csvLine[2]] = Chart1D(GetNextColor(), count );
				m_totalTime[csvLine[2]] = Chart1D(GetNextColor(), csvLine[5].getValueN<double>() );
			}
		}
		else if( csvLine.size() == 6 && csvLine[0] != "file" )
		{
			Chart2D	&chart = m_timingsData[csvLine[2]];
			if( !chart.data.size() )
			{
				chart.color = GetNextColor();
				chart.lineWidth = 2;
			}
			chart.data.addElement( Chart2dPoint( chart.data.size(), csvLine[5].getValueN<double>() ) );
		}
	}
	return CheckFORM_form::create(nullptr);
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
ProcessStatus CheckMainWindow::handleCreate()
{
	FunctionNameBOX->setTag( -1 );
	for(
		TimingsData::const_iterator it = m_timingsData.cbegin(), endIT = m_timingsData.cend();
		it != endIT;
		++it
	)
	{
		if( it->getValue().data.size() > MIN_COUNT )
		{
			FunctionNameBOX->addEntry( it->getKey() );
			CHARTCHILD->add1dChart( Chart1D(GetNextColor(), it->getValue().data.size())  );
		}
//		CHARTCHILD->addChartLine(it->getValue());
	}
	return psDO_DEFAULT;
}

ProcessStatus CheckMainWindow::handleSelectionChange( int control )
{
	if( control == FunctionNameBOX_id )
	{
		STRING theFunction = FunctionNameBOX->getSelectedItems();
		Chart2D	&chart = m_timingsData[theFunction];
		if( chart.data.size() )
		{
			CHARTCHILD->clearData();
			CHARTCHILD->add2dChart(chart);
			CHARTCHILD->invalidateWindow();
		}
	}
	return psDO_DEFAULT;
}

ProcessStatus CheckMainWindow::handleButtonClick( int control )
{
	const TotalData	*totalData = nullptr;

	if( control == TimePUSHBUTTON_id )
	{
		totalData = &m_totalTime;
	}
	else if( control == CountPUSHBUTTON_id )
	{
		totalData = &m_totalCount;
	}
	if( totalData )
	{
		CHARTCHILD->clearData();
		for(
			TotalData::const_iterator it = totalData->cbegin(), endIT = totalData->cend();
			it != endIT;
			++it
		)
		{
			CHARTCHILD->add1dChart( it->getValue() );
		}
		CHARTCHILD->invalidateWindow();
		return psPROCESSED;
	}

	return CheckFORM_form::handleButtonClick( control );
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

