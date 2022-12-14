
#define SQR(x) ((x)*(x))	/* macro for square */
#ifndef M_PI			/* in case M_PI not found in math.h */
#define M_PI 3.1415927
#endif
#ifndef M_E
#define M_E 2.718282
#endif

#define MAX_FILENAME_CHARS	320

char	filename[MAX_FILENAME_CHARS];

HWND	MainWnd;

		// Display flags
int		ShowPixelCoords, ShowRegionGrow, Clear, Play, Step, TotalReg, Red, Green, Blue, Orange, Violet, Aqua, Yellow, Predicate1, Predicate2, flag;

// Image data
unsigned char* OriginalImage;
int				ROWS, COLS;

#define TIMER_SECOND	1			/* ID of timer used for animation */

// Drawing flags
int		TimerRow, TimerCol;
int		ThreadRow, ThreadCol;
int		ThreadRunning;
int		BigDots;
int		mouse_x, mouse_y;

// Function prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void PaintImage();
void RegionGrowThread(void*);		/* passes address of window */
void AnimationThread(void*);
void ExplosionThread(void*);		/* passes address of window */
void RegionGrow(void*);