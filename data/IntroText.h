#pragma once
static const char *introTextData =
	"\blThe Galaxy Toy - Version 0.9.0 - RCAProductionmaster@gmail.com, irc.freenode.net #powder username RCAProduction\n"
	"\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x7F\n"
	//"\baA\bbB\bcC\bdD\beE\bfF\bgG\bhH\biI\bjJ\bkK\blL\bmM\bnN\boO\bpP\bqQ\brR\bsS\btT\buU\bvV\bwW\bxX\byY\bzZ\n"
	"\brPress 'ctrl+H' or 'F1' to access this message again.\n"
	"\brPress 'shift+n' for news and help regarding this mod.\n"

	"\bgControl+C/V/X are Copy, Paste and cut respectively.\n"
	"\bgTo choose a material, hover over one of the icons on the right, it will show a selection of elements in that group.\n"
	"\bgPick your material from the menu using mouse left/right buttons.\n"
	"Draw freeform lines by dragging your mouse left/right button across the drawing area.\n"
	"Shift+drag will create straight lines of particles.\n"
	"Ctrl+drag will result in filled rectangles.\n"
	"Ctrl+Shift+click will flood-fill a closed area.\n"
	"Use the mouse scroll wheel, or '[' and ']', to change the tool size for particles.\n"
	"Middle click or Alt+Click to \"sample\" the particles.\n"
	"Ctrl+Z will act as Undo.\n"
	"\n\boUse 'Z' for the zoom tool. Click to make the zoom window stay. Use the mouse wheel to change the zoom strength.\n"
	"The spacebar can be used to pause physics.\n"
	"Use 'S' to save parts of the window as 'stamps'. 'L' will load the most recent stamp, 'K' shows a library of stamps you saved.\n"
	"'H' will toggle the HUD. 'D' will toggle debug mode. 'ctrl+shift+D' will toggle super debug mode.\n"
	"'P' will take a screenshot and save it into the current directory.\n"
	"\n"
	"Contributors: \bgStanislaw K Skowronek (Designed the original Powder Toy),\n"
	"\bgSimon Robertshaw, Skresanov Savely, cracker64, Catelite, Bryan Hoyle, Nathan Cousins, jacksonmj, Felix Wallin,\n"
	"\bgLieuwe Mosch, Anthony Boot, Matthew \"me4502\", MaksProg, jacob1, mniip, RCAProduction, the_new_powder99999\n"
	"\n"
	"\bgTo use online features such as saving, you need to register at: \brhttp://powdertoy.co.uk/Register.html \bgor\n"
	"\brhttp://FortCollinsB17.org:3000/register \bgdepending on your choice of servers.\n"

	"\n"
	"\bt" MTOS(SAVE_VERSION) "." MTOS(MINOR_VERSION) "." MTOS(BUILD_NUM) " " IDENT_PLATFORM " "
#ifdef SNAPSHOT
	"SNAPSHOT " MTOS(SNAPSHOT_ID) " "
#elif MOD_ID > 0
	"MODVER " MTOS(SNAPSHOT_ID) " "
#endif
#ifdef X86
	"X86 "
#endif
#ifdef X86_SSE
	"X86_SSE "
#endif
#ifdef X86_SSE2
	"X86_SSE2 "
#endif
#ifdef X86_SSE3
	"X86_SSE3 "
#endif
#ifdef LUACONSOLE
	"LUACONSOLE "
#endif
#ifdef GRAVFFT
	"GRAVFFT "
#endif
#ifdef REALISTIC
	"REALISTIC"
#endif
	;
