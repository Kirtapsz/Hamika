#define KIR4_MEMORY_C_LEAK_CHECK
#define KIR4_MEMORY_CPP_LEAK_CHECK

#include <KIR\KIR4_memory_c_def.h>
#include <KIR\KIR4_memory_cpp_def.h>
#include <KIR\KIR4_console.h>
#include <KIR\AL\KIR5_main.h>
#include <KIR\AL\KIR5_event.h>
#include <KIR\AL\KIR5_base.h>
#include <KIR\KIR4_string.h>
#include <KIR\AL\KIR5_panel.h>
#include <KIR\KIR4_time.h>
#include <KIR\AL\KIR5_bitmap.h>
#include <KIR\KIR4_system.h>
#include <KIR\AL\KIR5_event_engine.h>
#include <KIR\AL\KIR5_color.h>

#include "Bitmap.h"
#include "Font.h"
#include "MainEvent.h"
#include "EditorMainEvent.h"
#include "Global.h"

#include "BitmapPool.h"

/*
ollo move: 14,91-1,96 sec/57 block
ollo rotate: 7,45-2,86/(5*360)

csillag effekt korbe: 1.86-7.30/6* korbe


kövek move:
kövek rotate: 34,70-34,39

push:

atbujas: 6.4/28
szipopant: 5.81-6.96(infotr) 8,50-8,27(zold)

ollo 12,75 sec alatt tesz meg 57 blockot
robbano leeses 4,85 tesz meg 21 blockot
*/


/*


elektromossag ido: 3,19-1,86 sec
chance: 14,79sec /16 tick
min sleep: 6,07

1-
14.19
2-
11.20
20.92
3-
7.36
13.43
4-
9.74
20.53
5-
6.13
15.57
6-
9.21
16.57
7-
11.13
20.31
8-
8.89
19.89
9-
11.43





*/



std::shared_ptr<KIR5::EventEngine> eventEngine;

//std::vector<std::vector<int>> v;
//
//int a = 0;

int main(int, char *[])
{
	//v.resize(100000);
	//for (auto &it : v)
	//{
	//	it.resize(0x1000);
	//	for (auto &t : it)
	//	{
	//		t = rand() % 256;
	//	}
	//}

	//auto start = std::chrono::steady_clock::now();
	//std::for_each(v.begin(), v.end(), [](std::vector<int> &iv)
	//{
	//	std::for_each(iv.begin(), iv.end(), [](int &val)
	//	{
	//		a += val;
	//	});
	//});
	//auto end = std::chrono::steady_clock::now();
	//std::chrono::duration<double> elapsed_seconds = end - start;
	//std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	KIR5::initialize();

	mainEvent = std::shared_ptr<MainEvent>(new MainEvent());
	eventEngine = std::shared_ptr<KIR5::EventEngine>(new KIR5::EventEngine(mainEvent, CPS));

	//Editor::mainEvent = std::shared_ptr<Editor::MainEvent>(new Editor::MainEvent());
	//eventEngine = std::shared_ptr<KIR5::EventEngine>(new KIR5::EventEngine(Editor::mainEvent, CPS));

	eventEngine->start();

	eventEngine = nullptr;
	Editor::mainEvent = nullptr;
	mainEvent = nullptr;

	return 0;
}