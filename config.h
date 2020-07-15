/**
 * Based on DT's DWM with major customiazation
 */

/* See LICENSE file for copyright and license details. */
/* appearance */
#include <X11/XF86keysym.h>
static const unsigned int systrayspacing = 4;
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const unsigned int gappx = 0;    /* pixel gap between clients */
static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 1;            /* 0 means bottom bar */
static const int horizpadbar = 7;       /* horizontal padding for statusbar */
static const int vertpadbar = 7;        /* vertical padding for statusbar */
static const int swallowfloating = 1;   /* 1 means swallow floating windows by default */
static const char *fonts[] = {
    "JetBrainsMono Nerd Font Mono:size=10:antialias=true:autohint=true",
    "Hack:size=10:antialias=true:autohint=true",
    "JoyPixels:size=10:antialias=true:autohint=true"
};
static const unsigned int baralpha = 0xff;
static const unsigned int borderalpha = 0xff;
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {"#ABB2BF", "#1E2127", "#61AFEF"},
    [SchemeSel]  = {"#98C379", "#5C6370", "#000000"},
};
static const unsigned int alphas[][3] = {
    /*               fg      bg        border     */
    [SchemeNorm] = {OPAQUE, baralpha, borderalpha},
    [SchemeSel] = {OPAQUE, baralpha, borderalpha},
};

/* tagging */
static const char *tags[] = {"", "", "", "", "", "", "", "", ""};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating is_terminal     noswallow  monitor */
    // { "Gimp",     NULL,       NULL,       0,            1,           -1 },
    {"Brave-browser",     NULL,  NULL,               1 << 1, 0, 0, 0,  1},   
    {"TelegramDesktop",   NULL,  NULL,               1 << 2, 0, 0, 0,  1},   
    {"MEGAsync",          NULL,  NULL,               NULL,   0, 0, 1, -1},  
    {NULL,                NULL,  "Spotify Premium",  1 << 4, 0, 0, 0,  1},   
    {"Notion",            NULL,  NULL,               1 << 5, 0, 0, 0,  1},   
    {NULL,                NULL,  "Event Tester",     0,      0, 0, 1, -1},   
};

/* layout(s) */
static const float mfact = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints = 0; /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
    {"###", grid},
    {NULL, NULL},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},
#define CMD(cmd)                                                               \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] = "1"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-p", "Run", NULL};
static const char *termcmd[] = {"st", NULL};

static Key keys[] = {
    /* modifier              key                       function        argument */
    {MODKEY,                 XK_d,                     spawn,          {.v = dmenucmd}}, // dmenu
    {MODKEY,                 XK_Return,                spawn,          {.v = termcmd}}, // terminal
    {MODKEY,                 XK_b,                     togglebar,      {0}},
    {MODKEY | ShiftMask,     XK_j,                     rotatestack,    {.i = +1}},
    {MODKEY | ShiftMask,     XK_k,                     rotatestack,    {.i = -1}},
    {MODKEY,                 XK_j,                     focusstack,     {.i = +1}},
    {MODKEY,                 XK_k,                     focusstack,     {.i = -1}},
    {MODKEY,                 XK_i,                     incnmaster,     {.i = +1}},
    {MODKEY,                 XK_u,                     incnmaster,     {.i = -1}},
    {MODKEY | ControlMask,   XK_h,                     setmfact,       {.f = -0.05}},
    {MODKEY | ControlMask,   XK_l,                     setmfact,       {.f = +0.05}},
    {MODKEY | ControlMask,   XK_Return,                zoom,           {0}},
    {MODKEY | ControlMask,   XK_Tab,                   view,           {0}},
    {MODKEY,                 XK_q,                     killclient,     {0}},

    /* Change layout */
    {MODKEY,                 XK_Tab,                   cyclelayout,    {.i = -1}},
    {MODKEY | ShiftMask,     XK_Tab,                   cyclelayout,    {.i = +1}},
    {MODKEY,                 XK_space,                 setlayout,      {0}},
    {MODKEY | ShiftMask,     XK_space,                 togglefloating, {0}},

    /* Switch to specific layouts */
    {MODKEY,                 XK_t,                     setlayout,      {.v = &layouts[0]}},
    {MODKEY,                 XK_f,                     setlayout,      {.v = &layouts[1]}},
    {MODKEY,                 XK_m,                     setlayout,      {.v = &layouts[2]}},
    {MODKEY,                 XK_g,                     setlayout,      {.v = &layouts[3]}},
    {MODKEY,                 XK_0,                     view,           {.ui = ~0}},
    {MODKEY | ShiftMask,     XK_0,                     tag,            {.ui = ~0}},

    /* Switching between monitors */
    {MODKEY,                 XK_l,                     focusmon,       {.i = -1}},
    {MODKEY,                 XK_h,                     focusmon,       {.i = +1}},
    {MODKEY | ShiftMask,     XK_l,                     tagmon,         {.i = -1}},
    {MODKEY | ShiftMask,     XK_h,                     tagmon,         {.i = +1}},

    /* Launch apps */
    {MODKEY,                 XK_z,                     spawn,          CMD("zathura")},
    {MODKEY,                 XK_w,                     spawn,          CMD("$BROWSER")},
    {MODKEY,                 XK_e,                     spawn,          CMD("dmenuunicode")},

    /* Control music */
    {MODKEY,                 XK_Right,                 spawn,          CMD("playerctl next")},
    {MODKEY,                 XK_Left,                  spawn,          CMD("playerctl previous")},
    {MODKEY,                 XK_Down,                  spawn,          CMD("playerctl play-pause")},
    {0,                      XF86XK_AudioPlay,         spawn,          CMD("playerctl play-pause")},

    TAGKEYS(XK_1, 0)
    TAGKEYS(XK_2, 1)
    TAGKEYS(XK_3, 2)
    TAGKEYS(XK_4, 3)
    TAGKEYS(XK_5, 4)
    TAGKEYS(XK_6, 5)
    TAGKEYS(XK_7, 6)
    TAGKEYS(XK_8, 7)
    TAGKEYS(XK_9, 8)
    {MODKEY | ShiftMask,     XK_q,                     quit,           {0}},
    {MODKEY | ShiftMask,     XK_r,                     quit,           {1}},
    /* System management */
    {0,                      XF86XK_AudioMute,         spawn,          CMD("pamixer -t && pkill -RTMIN+12 dwmblocks")},
    {0,                      XF86XK_AudioRaiseVolume,  spawn,          CMD("vol -u && pkill -RTMIN+12 dwmblocks")},
    {0,                      XF86XK_AudioLowerVolume,  spawn,          CMD("vol -d && pkill -RTMIN+12 dwmblocks")},
    {0,                      XK_Print,                 spawn,          CMD("wscr -f")},
    {ControlMask,            XK_Print,                 spawn,          CMD("wscr")},
    {0,                      XF86XK_MonBrightnessUp,   spawn,          CMD("brightness --up && pkill -RTMIN+11 dwmblocks")},
    {0,                      XF86XK_MonBrightnessDown, spawn,          CMD("brightness --down && pkill -RTMIN+11 dwmblocks")},
    {0,                      XF86XK_TouchpadToggle,    spawn,          CMD("touchpad-toggle")},
    {Mod1Mask | ControlMask, XK_l,                     spawn,          CMD("slock")},
    {Mod1Mask | ShiftMask,   XK_l,                     spawn,          CMD("dpower")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click           event mask   button          function        argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
