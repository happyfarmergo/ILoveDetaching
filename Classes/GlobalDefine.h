#ifndef _GLOBALDEFINE_H_
#define _GLOBALDEFINE_H_

#define COLUMN 8
#define ROW 8

#define BOX_SIZE 48
#define BOADER_SIZE 10

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 480

#define BOX_NUM 6
#define DEADLINETIME 600
#define NOTIFY NotificationCenter::getInstance()

#define SOUND_KEY "sound_key"
#define MUSIC_KEY "music_key"
#define USER_DEFAULT UserDefault::getInstance()

#define DATALAYER 12
#define BOXMANAGER 10
#define LABELLAYER 13
#define LABELTIME 6
#define LABELSCORE 8

static const char *BoxNormal[BOX_NUM] = {
	"bls.png",
	"dls.png",
	"ems.png",
	"jbs.png",
	"jcs.png",
	"ygs.png"
};

static const char *BoxHorizontal[BOX_NUM] = {
	"bls_h.png",
	"dls_h.png",
	"ems_h.png",
	"jbs_h.png",
	"jcs_h.png",
	"ygs_h.png"
};

static const char *BoxVertical[BOX_NUM] = {
	"bls_v.png",
	"dls_v.png",
	"ems_v.png",
	"jbs_v.png",
	"jcs_v.png",
	"ygs_v.png"
};

static const char *BoxTriangle[BOX_NUM] = {
	"bls_t.png",
	"dls_t.png",
	"ems_t.png",
	"jbs_t.png",
	"jcs_t.png",
	"ygs_t.png"
};

static const char *BoxEaten = "pkq.png";

#endif