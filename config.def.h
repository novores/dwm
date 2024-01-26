/* See LICENSE file for copyright and license details. */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* appearance */
static const unsigned int borderpx        = 0;        /* border pixel of windows */
static const unsigned int gappx           = 5;        /* gaps between windows */
static const unsigned int snap            = 32;       /* snap pixel */
static const unsigned int systraypinning  = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft   = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing  = 2;   /* systray spacing */
static const int systraypinningfailfirst  = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray              = 1;     /* 0 means no systray */
static const int showbar                  = 1;        /* 0 means no bar */
static const int topbar                   = 1;        /* 0 means bottom bar */
static const double activeopacity         = 1.0f;     /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity       = 1.0f;   /* Window opacity when it's inactive (0 <= opacity <= 1) */
static       Bool bUseOpacity             = True;     /* Starts with opacity on any unfocused windows */
static const char *fonts[]                = { "JetBrainsMono NF:pixelsize=12:antialias=true:autohint=true" };
static char dmenufont[]                   = "JetBrainsMono NF:pixelsize=12:antialias=true:autohint=true";
static const char normbgcolor[]           = "#1E1E2E";
static const char normfgcolor[]           = "#585767";
static const char normbordercolor[]       = "#f8f8f2";
static const char selbgcolor[]            = "#1E1E2E";
static const char selfgcolor[]            = "#ABE9B3";
static const char selbordercolor[]        = "#ABE9B3";
static const char col_urgborder[]         = "#F28FAD";
static const char *colors[][3]            = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbgcolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  normbgcolor  },
       [SchemeTitle]  = { selfgcolor, selbgcolor, selbordercolor  },
       [SchemeUrg]  = { selbordercolor, selfgcolor,  col_urgborder  },
};

static const char *const autostart[] = {
  "feh", "--bg-fill", "/home/novores/Pictures/wallpapers/su-san-lee-E_eWwM29wfU-unsplash.jpg", NULL,
  "/usr/lib/mate-polkit/polkit-mate-authentication-agent-1", NULL,
  "xbanish", NULL,
  "usermount", NULL,
  "xbacklight", "-set", "50", NULL,
  "sh", "-c", "xss-lock -n ~/.local/bin/dim-screen.sh --transfer-sleep-lock -- betterlockscreen --lock &", NULL,
  "xrdb", "/home/novores/.Xresources", NULL,
  "sh", "-c", "/home/novores/.local/bin/dwmstatus", NULL,
  "sh", "-c", "/home/novores/.local/bin/battery.sh", NULL,
  // "xcompmgr", "-c", "-C", "-t-5", "-l-5", "-r4.2", "-o.55", NULL,
  "picom", NULL,
  "setxkbmap", "-option", "grp:alt_shift_toggle", "us,ara", NULL,
	NULL /* terminate */
};

static const char *tagsel[][2] = {
   /*   fg         bg    */
  { normfgcolor, normbgcolor }, /* norm */
  { selfgcolor, selbgcolor  }, /* sel */
  { normbordercolor,  normbgcolor }, /* occ but not sel */
  { selbgcolor,  normfgcolor }, /* has pinned tag */
};
/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6" };
static const char *tagsalt[] = { "", "", "", "", "", "" };
static const int momentaryalttags = 0; /* 1 means alttags will show only when key is held down*/

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 0;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                    instance    title       tags mask     isfloating   monitor */
  {"qutebrowser",             NULL,       NULL,       1 << 1,       0,           -1},
  {"firefox",                 NULL,       NULL,       1 << 1,       0,           -1},
  {"Soffice",                 NULL,       NULL,       1 << 2,       0,           -1},
  {"Inkscape",                NULL,       NULL,       1 << 3,       0,           -1},
  {"Gimp",                    NULL,       NULL,       1 << 3,       0,           -1},
  {"SimpleScreenRecorder",    NULL,       NULL,       0,            1,           -1},
  {"mpv",                     NULL,       NULL,       0,            1,           -1},
  {"feh",                     NULL,       NULL,       0,            1,           -1},
  {"Lxappearance",            NULL,       NULL,       0,            1,           -1},
  {"qt5ct",                   NULL,       NULL,       0,            1,           -1},
  {"aft-linux-qt",            NULL,       NULL,       0,            1,           -1},
  {"KeePassXC",               NULL,       NULL,       0,            1,           -1},
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

// library keyxf86
#include <X11/XF86keysym.h>

/* commands */
static char dmenumon[2] = "1"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-p", "Apps", "-m", dmenumon, "-fn", dmenufont, "-nb", selfgcolor, "-nf", normbgcolor, "-sb", selbgcolor, "-sf", selfgcolor, "-hp", "firefox,inkscape,qutebrowser,localc,lobase,lowriter,loimpress,android-file-transfer,gimp,nextcloud,alacritty,steam,obs,shotcut,pavucontrol,lxappearance,poweroff,reboot,sleep", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *volup[] = {"pamixer", "-i", "2", NULL};
static const char *voldown[] = {"pamixer", "-d", "2", NULL};
static const char *mute[] = {"pamixer", "-t", NULL};
static const char *mpcpl[] = {"mpc", "toggle", NULL};
static const char *mpcnext[] = {"mpc", "next", NULL};
static const char *mpcprev[] = {"mpc", "prev", NULL};
static const char *brup[] = {"xbacklight", "-inc", "2", NULL};
static const char *brdown[] = {"xbacklight", "-dec", "2", NULL};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_a,      toggleopacity,  {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_n,      togglealttag,   {0} },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
  { MODKEY|ControlMask,           XK_q,        quit,         {0} }, 
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

  //command using xf86 keys
  {0,                             XF86XK_AudioMute,           spawn,  {.v = mute}},
  {0,                             XF86XK_AudioRaiseVolume,    spawn,  {.v = volup}},
  {0,                             XF86XK_AudioLowerVolume,    spawn,  {.v = voldown}},
  {0,                             XF86XK_AudioPlay,           spawn,  {.v = mpcpl}},
  {0,                             XF86XK_AudioNext,           spawn,  {.v = mpcnext}},
  {0,                             XF86XK_AudioPrev,           spawn,  {.v = mpcprev}},
  {0,                             XF86XK_MonBrightnessUp,     spawn,  {.v = brup}},
  {0,                             XF86XK_MonBrightnessDown,   spawn,  {.v = brdown}},

  // Misc
  {ALTKEY,                        XK_m,       spawn,        SHCMD("[[ $(ls $HOME/.local/share/mpd/pid) ]] && mpd --kill || mpd")},
  {ALTKEY,                        XK_Down,    spawn,        SHCMD("mpc stop")},
  {MODKEY,                        XK_F1,       spawn,        SHCMD("rbwmenu")},
  {MODKEY,                        XK_F2,       spawn,        SHCMD("transmenu")},
  {MODKEY,                        XK_F3,       spawn,        SHCMD("dmenu-bluetooth")},
  {MODKEY,                        XK_F4,       spawn,        SHCMD("st -e broot")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
