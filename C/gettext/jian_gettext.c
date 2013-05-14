#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>

#define _(S) gettext(S)
#define PACKAGE "jian_gettext"

int main(void) {
	char str[100];
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, "locale");
	textdomain(PACKAGE);

	strcpy(str, _("will be copy\n"));

	printf(_("Hello, gettext!\n"));
	printf("%s", _(str));

	return 0;
}

