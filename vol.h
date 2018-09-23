#ifndef vol_h_
#define vol_h_

struct model_vol {
	char text[21];
	int places;
};
typedef struct model_vol VOL;

struct msgbuf{
		long mtype;
		VOL vol;
	};
typedef struct msgbuf MESSAGE;

#endif
