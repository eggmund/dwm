/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include <stdio.h>

static const unsigned int gap = 30;
/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Source Code Pro:size=10" };
static const char dmenufont[]       = "Source Code Pro:size=10";
/* Bar settings */
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int gappx     = gap;        /* gap pixel between windows */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = gap;       /* horizontal padding of bar */

// static const char col_gray1[]       = "#222222";
// static const char col_gray2[]       = "#444444";
// static const char col_gray3[]       = "#bbbbbb";
// static const char col_gray4[]       = "#eeeeee";
// static const char col_cyan[]        = "#005577";
// static const char col_cadet[]        = "#56666B";
// static const char col_background[] = "#888f63"; // "#629460"

// gruvbox colours https://github.com/morhetz/gruvbox
static const char col_gruv_bg0_s[] = "#32302f";
static const char col_gruv_bg1[] = "#3c3836";
static const char col_gruv_bg2[] = "#504945";
static const char col_gruv_bg3[] = "#665c54";
static const char col_gruv_bg4[] = "#7c6f64";
static const char col_gruv_green[] = "#b8bb26";
static const char col_gruv_green2[] = "#98971a";
static const char col_gruv_red[] = "#fb4934";
static const char col_gruv_yellow[] = "#fabd2f";
static const char col_gruv_blue[] = "#83a598";
static const char col_gruv_blue2[] = "#458588";
static const char col_gruv_purple[] = "#d3869b";
static const char col_gruv_aqua[] = "#8ec07c";
static const char col_gruv_fg1[] = "#ebdbb2";
static const char col_gruv_fg2[] = "#d5c4a1";
static const char col_gruv_fg3[] = "#bdae93";

static const char *colors[][3] = {
	/*               fg         bg         border   */
	//[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	//[SchemeSel]  = { col_gray4, col_background,  col_background  },
	[SchemeNorm] = { col_gruv_fg1, col_gruv_bg0_s, col_gruv_bg1 },
	[SchemeSel]  = { col_gruv_fg1, col_gruv_blue,  col_gruv_blue },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating  ispermanent  monitor */
	{ "Gimp",     NULL,       NULL,       0,            True,       False,       -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       False,      False,       -1 },
	{ "status_bar",  NULL,       NULL,       0,       False,      True,       -1 },
};

/* layout(s) */
#include "fibonacci.c"

static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",      dwindle },
};


/* key definitions */
#define SUPER Mod4Mask
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char dmenu_x[] = "30";	// gap
static const char dmenu_y[] = "10";	// vertpad
static const char dmenu_w[] = "1860";	// 1920 - gap * 2

static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", col_gruv_bg0_s, "-nf", col_gruv_fg2, "-sb", col_gruv_blue, "-sf", col_gruv_fg1, NULL };
static const char *termcmd[]  = { "termite", NULL };
static const char *browsercmd[] = { "qutebrowser", NULL };
static const char *flameshotcmd[] = { "flameshot", "gui", NULL };
static const char *sleepcmd[] = { "systemctl", "suspend", NULL };
static const char *filebrowsercmd[] = { "thunar", NULL };
static const char *discordcmd[] = { "discord", NULL };
static const char *discordkillcmd[] = { "/usr/bin/killd", NULL };
	/* Multimedia */
static const char *play_pausecmd[] = { "playerctl", "play-pause", NULL };
static const char *previouscmd[] = { "playerctl", "previous", NULL };
static const char *nextcmd[] = { "playerctl", "next", NULL };
static const char *togglemutecmd[] = { "amixer", "set", "Capture", "toggle", NULL };
static const char *togglemutemaincmd[] = { "amixer", "set", "Master", "toggle", NULL };
static const char *raisevolumecmd[] = { "amixer", "set", "Master", "5%+", NULL };
static const char *lowervolumecmd[] = { "amixer", "set", "Master", "5%-", NULL };
static const char *pavucmd[] = {"pavucontrol", NULL};
	/* Laptop Brightness */
static const char *brightness_upcmd[] = { "light", "-A", "5", NULL };
static const char *brightness_downcmd[] = { "light", "-U", "5", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,				XK_h,	   spawn,		   {.v = sleepcmd } },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,		XK_v,	   spawn,		{.v = pavucmd} },
	{ 0,							XF86XK_MonBrightnessUp,	spawn,	{.v = brightness_upcmd } },
	{ 0,							XF86XK_MonBrightnessDown,	spawn,	{.v = brightness_downcmd } },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	// { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[3]} },
	// { MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ SUPER|ShiftMask,             XK_r,      quit,           {0} },	/* R for Reload, since i have it started in while true loop */
	{ MODKEY,						XK_g,	   spawn,		   {.v = browsercmd } },
	{ MODKEY,						XK_c,	   spawn,		   {.v = filebrowsercmd } },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask,				XK_d,	   spawn,		   {.v = discordcmd} },
	{ SUPER|ShiftMask,				XK_d,	   spawn,		   {.v = discordkillcmd} },
	{ MODKEY,						XK_w,	   spawn,		   {.v = flameshotcmd } },
	{ 0,							XF86XK_AudioPlay,	spawn,	{.v = play_pausecmd} },
	{ 0,							XF86XK_AudioPrev,	spawn,	{.v = previouscmd } },
	{ 0,							XF86XK_AudioNext,	spawn,	{.v = nextcmd } },
	{ 0,							XK_Pause,	spawn,	{.v = togglemutecmd } },
	{ 0,							XF86XK_AudioMute,	spawn,	{.v = togglemutemaincmd } },
	{ 0,							XF86XK_AudioRaiseVolume,	spawn,	{.v = raisevolumecmd } },
	{ 0,							XF86XK_AudioLowerVolume,	spawn,	{.v = lowervolumecmd } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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

