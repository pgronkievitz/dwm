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
static const int horizpadbar = 0;       /* horizontal padding for statusbar */
static const int vertpadbar = 7;        /* vertical padding for statusbar */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const char *fonts[] = {
    "Hurmit Nerd Font Mono:size=10:antialias=true:autohint=true",
    "Hack:size=10:antialias=true:autohint=true",
    "JoyPixels:size=10:antialias=true:autohint=true"};
static const char col_gray1[] = "#262626";
static const char col_gray2[] = "#262626"; /* border color unfocused windows */
static const char col_gray4[] = "#d0d0d0";
static const char col_cyan[] =
    "#00afaf"; /* border color focused windows and tags */
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = 0xcc;
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {"#ABB2BF", "#1E2127", "#61AFEF"},
    [SchemeSel]  = {"#98C379", "#5C6370", "#000000"},
    [SchemeBat]  = {"#198844", "#1d1f21", "#000000"},
    [SchemeBri]  = {"#FBA922", "#1d1f21", "#000000"},
    [SchemeVol]  = {"#3971ED", "#1d1f21", "#000000"},
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
    /* class      instance    title       tags mask     isfloating   monitor */
    // { "Gimp",     NULL,       NULL,       0,            1,           -1 },
    {"Brave-browser",     NULL,  NULL,  1 << 1,  0, 1},   
    {"qutebrowser",       NULL,  NULL,  1 << 1,  0, 1},   
    {"TelegramDesktop",   NULL,  NULL,  1 << 2,  0, 1},   
    {"MEGAsync",          NULL,  NULL,  NULL  ,  1, -1},  
    {"Spotify",           NULL,  NULL,  1 << 4,  0, 1},   
    {"Todoist",           NULL,  NULL,  1 << 5,  0, 1},   
};

/* layout(s) */
static const float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    0; /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle}, {"###", grid}, {NULL, NULL},
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
static const char *screenshot[] = {"wscr", "-f", NULL};
static const char *ssselect[] = {"wscr", NULL};

static Key keys[] = {
    /* modifier             key        function        argument */
    {MODKEY, XK_d, spawn, {.v = dmenucmd}}, // dmenu
    {MODKEY, XK_Return, spawn, {.v = termcmd}}, // terminal
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY | ShiftMask, XK_j, rotatestack, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, rotatestack, {.i = -1}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_u, incnmaster, {.i = -1}},
    {MODKEY | ControlMask, XK_h, setmfact, {.f = -0.05}},
    {MODKEY | ControlMask, XK_l, setmfact, {.f = +0.05}},
    {MODKEY | ControlMask, XK_Return, zoom, {0}},
    {MODKEY | ControlMask, XK_Tab, view, {0}},
    {MODKEY, XK_q, killclient, {0}},

    /* Layout manipulation */
    {MODKEY, XK_Tab, cyclelayout, {.i = -1}},
    {MODKEY | ShiftMask, XK_Tab, cyclelayout, {.i = +1}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},

    /* Switch to specific layouts */
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_g, setlayout, {.v = &layouts[3]}},

    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},

    /* Switching between monitors */
    {MODKEY, XK_l, focusmon, {.i = -1}},
    {MODKEY, XK_h, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_l, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_h, tagmon, {.i = +1}},

    /* Apps Launched with SUPER + ALT + KEY */
    {MODKEY | Mod1Mask, XK_h, spawn, CMD("st -e ytop")}, // task manager
    {MODKEY | Mod1Mask, XK_n, spawn, CMD("st -e newsboat")}, // newsboat
    {MODKEY | Mod1Mask, XK_r, spawn, CMD("st -e tuir --enable-media")}, // reddit
    {MODKEY | Mod1Mask, XK_m, spawn, CMD("spotify")}, // spotify
    {MODKEY | Mod1Mask, XK_t, spawn, CMD("todoist")}, // todoist
    {MODKEY | Mod1Mask, XK_s, spawn, CMD("settings")},
    {MODKEY | Mod1Mask, XK_w, spawn, CMD("st -e weather")},

    /* Dmenu scripts launched with ALT + CTRL + KEY */
    {MODKEY, XK_z, spawn, CMD("zathura")},
    {MODKEY, XK_w, spawn, CMD("$BROWSER")},
    {Mod1Mask | ControlMask, XK_l, spawn, CMD("slock")},
    {Mod1Mask | ControlMask, XK_s, spawn, CMD("st -e BROWSER=w3m ddgr $(dmenu -p \"Search\")")},
    {Mod1Mask | ControlMask | ShiftMask, XK_l, spawn, CMD("/home/pg/.local/bin/dpower")},
    {MODKEY, XK_Right, spawn, CMD("playerctl next && pkill -RTMIN+10 dwmblocks")},
    {MODKEY, XK_Left, spawn, CMD("playerctl previous && pkill -RTMIN+10 dwmblocks")},
    {MODKEY, XK_Down, spawn, CMD("playerctl play-pause && pkill -RTMIN+10 dwmblocks")},
    {0, XF86XK_AudioPlay, spawn, CMD("playerctl play-pause && pkill -RTMIN+10 dwmblocks")},

    TAGKEYS(XK_1, 0)
    TAGKEYS(XK_2, 1)
    TAGKEYS(XK_3, 2)
    TAGKEYS(XK_4, 3)
    TAGKEYS(XK_5, 4)
    TAGKEYS(XK_6, 5)
    TAGKEYS(XK_7, 6)
    TAGKEYS(XK_8, 7)
    TAGKEYS(XK_9, 8)
    {MODKEY | ShiftMask, XK_q, quit, {0}},
    {MODKEY | ShiftMask, XK_r, quit, {1}},
    {0, XF86XK_AudioMute, spawn, CMD("/usr/bin/pactl set-s 12ink-mute 0 toggle && pkill -RTMIN+12 dwmblocks")},
    {0, XF86XK_AudioRaiseVolume, spawn, CMD("vol --up && pkill -RTMIN+12 dwmblocks")},
    {0, XF86XK_AudioLowerVolume, spawn, CMD("vol --down && pkill -RTMIN+12 dwmblocks")},
    {0, XF86XK_AudioMute, spawn, CMD("pamixer -t && pkill -RTMIN+12 dwmblocks")},
    {0, XK_Print, spawn, {.v = screenshot}}, {ControlMask, XK_Print, spawn, {.v = ssselect}},
    {0, XF86XK_MonBrightnessUp, spawn, CMD("brightness --up && pkill -RTMIN+11 dwmblocks")},
    {0, XF86XK_MonBrightnessDown, spawn, CMD("brightness --down && pkill -RTMIN+11 dwmblocks")},
    {0, XF86XK_TouchpadToggle, spawn, CMD("touchpad-toggle")},
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
