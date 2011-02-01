#include "ls.h"

#define COLOR_BUFSIZE	26

/*Color value from command dircolors*/
static const char ls_colors[]="rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lz=01;31:*.xz=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.rar=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.jpg=01;35:*.jpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.axv=01;35:*.anx=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.axa=00;36:*.oga=00;36:*.spx=00;36:*.xspf=00;36:";

int print_color_name(const char *mode, const char *filename) {
	char color_v[COLOR_BUFSIZE + 1];
	char *c_s, *c_e, *file_ext;
	int n;

	switch (mode[0]) {
	case '\0':
		return 0;
	case 'd':
		file_ext = "di";
		break;
	case 's':
		file_ext = "so";
		break;
	case 'l':
		file_ext = "ln";
		break;
	case 'b':
		file_ext = "bd";
		break;
	case 'c':
		file_ext = "cd";
		break;
	case 'f':
		file_ext = "pi";
		break;
	default:
		if (mode[3] == 'x' || mode[6] == 'x' || mode[9] == 'x' )
			file_ext = "ex";
		else if (mode[3] == 's' || mode[3] == 'S')
			file_ext = "su";
		else if (mode[6] == 's' || mode[6] == 'S')
			file_ext = "sg";
		else if (mode[9] == 't' || mode[9] == 'T')
			file_ext = "st";
		else if ((file_ext = strrchr(filename, '.')) == NULL)	 /**/
			file_ext = "rs"; //return 0;
		break;
	}

	if ((c_s = strstr(ls_colors, file_ext)) == NULL)
		return 0;
	if	((c_e = strchr(c_s, ':')) == NULL)
		return 0;
	n = c_e - c_s;
	if (n > COLOR_BUFSIZE)
		return 0;
	strncpy(color_v, c_s, n);
	color_v[n] = '\0';
	if ((c_s = strchr(color_v, '=')) == NULL )
		return 0;

	printf("\033[%sm%s\033[0m", c_s + 1, filename);
	return 1;
}
