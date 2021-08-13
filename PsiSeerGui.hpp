#ifndef INCLUDE_HPP_PSISEERGUI_
#define INCLUDE_HPP_PSISEERGUI_


class Fl_Widget;
class Fl_Double_Window;
class Fl_Button;
class Fl_Box;
class Fl_Round_Button;
class Fl_Output;
class Fl_Group;
class Fl_Input;
class Fl_Value_Slider;
class PsiSeerApp;
class Fl_Light_Button;
class Fl_Menu_Bar;
class Fl_PNG_Image;
class Fl_Check_Button;
struct Fl_Menu_Item;

class SessionPlot;

#include <FL/Enumerations.H>
#include "test/Threads.H"
#include "LpFilter.hpp"
#include "PsiStats.h"
#include <string>

class PsiSeerGui
{
public:
    enum Color { COLOR_DEFAULT = FL_BACKGROUND2_COLOR, COLOR_RED = FL_RED, COLOR_GREEN = FL_GREEN };

public:
    PsiSeerGui(PsiSeerApp* psiSeerApp);
    ~PsiSeerGui();

public:
    void ShowBusy();
    void ShowHit();
    void ShowMiss();
    
    void SetTrialStats(double total, double hitRate, double pValue);
    void SetWinStats(double hitRate, double pValue);
    void SetSlider(double sliderVal);
    void SetRate(double newRate, double length);
    void CloseAllFrames();

    bool IsPracticeMode();
    bool IsMuted();
    bool IsMissMuted();
    bool IsMissVisible();
    bool IsTargetInverted();

    void SetSource(char* source);
    bool IsShowing();

private:
    void VersionTest();

    static void Command(Fl_Widget* widget, void* instance);
    static void ToggleLogging(Fl_Widget* widget, void* instance);
    static void Authenticate(Fl_Widget* widget, void* instance);
    static void Kill(Fl_Widget* widget, void* instance);
    static void SelfTest(Fl_Widget* widget, void* object);
    static void SelfTestNoLog(Fl_Widget* widget, void* object);

	static void OnNewCommand(Fl_Widget* widget, void* object);
	static void OnSendCommand(Fl_Widget* widget, void* object);

    static void ShowHelp(Fl_Widget* widget, void* instance);
    static void HideHelp(Fl_Widget* widget, void* instance);
    static void ShowQStart(Fl_Widget* widget, void* object);
    static void HideQStart(Fl_Widget* widget, void* instance);
    static void Closer(Fl_Widget* widget, void* object);

    static void TogglePracticeMode(Fl_Widget* widget, void* object);
    static void ToggleStatsView(Fl_Widget* widget, void* object);
    static void TogglePsiLevelView(Fl_Widget* widget, void* object);
    static void ResetStats(Fl_Widget* widget, void* object);
    static void ToggleSounds(Fl_Widget* widget, void* object);
    static void ToggleMissSound(Fl_Widget* widget, void* object);
    static void ToggleMissBall(Fl_Widget* widget, void* object);
    static void ToggleTargetDirection(Fl_Widget* widget, void* object);
    static void ToggleAbout(Fl_Widget* widget, void* object);

    static void* SetPsiModePK(Fl_Widget* widget, void* object);
    static void* SetPsiModeCV(Fl_Widget* widget, void* object);
    static void* SetPsiModePC(Fl_Widget* widget, void* object);
    static void* SourceMenu(Fl_Widget* widget, void* object);
    static void* SetSourceAuto(Fl_Widget* widget, void* object);
    static void* SetSourcePcqn(Fl_Widget* widget, void* object);
    static void* SetSourceHdwr(Fl_Widget* widget, void* object);
    static void* SetSourceHdwrAc(Fl_Widget* widget, void* object);
    static void* SetSourcePrng(Fl_Widget* widget, void* object);
    static const char* LoadHelpUrl(Fl_Widget* widget, const char* url);

    static void ShowSessionPlot(Fl_Widget* widget, void* object);
    static void ShowProgressPlot(Fl_Widget* widget, void* object);

    static void* AnimationThread(void* instance);
    static void* SelfTestThread(void* instance);
//    static void* SetPsiMode(void* object);

	int BitRateFromSerial(std::string id); // (NEW)
	int BufferSizeFromSerial(std::string id); // (NEW)
	int BoundedThresholdFromSerial(std::string id); // (NEW)

private:
    Fl_Double_Window* mainWindow_;
    Fl_Group* psiBox_;

    Fl_Group* loginDialog_;
    Fl_Input* userIdBox_;
    Fl_Input* passcodeBox_;
    Fl_Button* loginButton_;
    Fl_Button* selfTestEndButton_;
    Fl_Check_Button* checkSocketsApi_;
    Fl_Check_Button* checkBrw_;

	Fl_Input* cmdBox_;
	Fl_Button* sendCmdButton_;

    Fl_Group* aboutDialog_;
    bool isAboutShown_;

    Fl_Value_Slider* psiValSlider_;
    Fl_Box* psiValMax_;
    Fl_Box* valSliderLabel_;

    Fl_Box* hitBox_;
    Fl_Box* missBox_;

    PsiSeerApp* psiSeerApp_;
    Fl_Thread commandThread_;
    Fl_Thread animationThread_;
    Fl_Thread selfTestThread_;

    Fl_Double_Window* helpWindow_;
    Fl_Double_Window* qStartWindow_;
//    Fl_Button* helpShowButton_;
//    Fl_Light_Button* invertButton_;
//    Fl_Group* trialStats_;

    Fl_Menu_Bar* menuBar_;

    bool inPracticeMode_;
    Fl_Box* practiceModeIndicator_;
    Fl_Box* statusIndicator_;
    Fl_Box* modeIndicator_;
    Fl_Box* sourceIndicator_;
    Fl_Box* commandLayer_;

    Fl_Output* trialN_;
    Fl_Output* trialHr_;
    Fl_Output* trialPHr_;
    Fl_Output* windowHr_;
    Fl_Output* windowPHr_;
    bool areStatsVisible_;
    bool arePsiLevelVisible_;
    Fl_Group* trialStats_;
    bool isMuted_;
	bool isMissMuted_;
	bool isMissVisible_;
    bool isTargetInverted_;
    bool isSelfTesting_;
	bool haveHardware_;
    Fl_Button* resetStats_;

    unsigned long aniThreadRunValidator_;

    SessionPlot* sessionPlot_;

    LpFilter rotationFilter_;
    double windowP_;
    CPsiStats psiStats_;

    Fl_PNG_Image* chevronLargeUp_;
    Fl_PNG_Image* chevronLargeDown_;

	std::string procTyp_; // (NEW)
};


#endif // INCLUDE_HPP_PSISEERGUI_
