// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#include "MfdMap_old.h"
#include "Pane.h"
#include "Psys.h"
#include "Celbody.h"
#include "Planet.h"
#include "Base.h"

using namespace std;

extern Orbiter *g_pOrbiter;
extern PlanetarySystem *g_psys;
extern InputBox *g_input;
extern Select *g_select;

// =======================================================================
// class Instrument_Map

struct Instrument_MapOld::SavePrm Instrument_MapOld::saveprm = {0,0,0,0,false,true};

Instrument_MapOld::Instrument_MapOld (Pane *_pane, INT_PTR _id, const Spec &spec, Vessel *_vessel)
: Instrument (_pane, _id, spec, _vessel)
{
	map = bgmap = 0;
	mapw = IW, maph = IW/2;
	mapx = mapy = 0;
	refplanet = 0;
	btgt      = 0;
	otgt      = 0;
	zoom      = false;
	track     = true;
	if (_vessel == saveprm.usr) {
		refplanet = saveprm.ref;
		btgt      = saveprm.btgt;
		otgt      = saveprm.otgt;
		zoom      = saveprm.zoom;
		track     = saveprm.track;
		if (!track) {
			mapx = (int)(saveprm.cx*IW);
			mapy = (int)(saveprm.cy*(IW/2));
		}
	}
	if (!refplanet) refplanet = _vessel->ProxyPlanet();
	if (!btgt)      btgt      = _vessel->LandingTarget();

	strcpy (title, "Map: ");
	strcpy (datastr[0], "Target base: ");
	strcpy (datastr[1], "Target orbit: ");

	if (gc) {
		//pen[0] = gc->clbkCreatePen (1, 1, col_yellow1);
		//pen[1] = gc->clbkCreatePen (1, 1, col_green1);
		//pen[2] = gc->clbkCreatePen (1, 1, RGB(255,128,128));
		//pen[3] = gc->clbkCreatePen (1, 1, RGB(255,255,255));
		brush[0] = gc->clbkCreateBrush (col_green1);
		brush[1] = gc->clbkCreateBrush (col_yellow1);
	}

	double dphi = Pi2/(double)INSTRMAP_NPROJPT;
	for (int i = 0; i < INSTRMAP_NPROJPT; i++) {
		double phi = i*dphi;
		cosp[i] = cos(phi), sinp[i] = sin(phi);
	}
	needmap = (refplanet != 0);
	SetSize (spec);
}

Instrument_MapOld::~Instrument_MapOld ()
{
	int i;

	// save status
	saveprm.usr   = vessel;
	saveprm.ref   = refplanet;
	saveprm.btgt  = btgt;
	saveprm.otgt  = otgt;
	saveprm.zoom  = zoom;
	saveprm.track = track;
	saveprm.cx    = (float)mapx/(float)mapw;
	saveprm.cy    = (float)mapy/(float)maph;
	if (map)   gc->clbkReleaseSurface (map);
	if (bgmap) gc->clbkReleaseSurface (bgmap);

	if (gc) {
		//for (i = 0; i < 4; i++)
		//	if (pen[i]) gc->clbkReleasePen (pen[i]);
		for (i = 0; i < 2; i++)
			if (brush[i]) gc->clbkReleaseBrush (brush[i]);
	}
}

HELPCONTEXT *Instrument_MapOld::HelpTopic () const
{
	extern HELPCONTEXT DefHelpContext;
	DefHelpContext.topic = (char*)"/mfd_map.htm";
	return &DefHelpContext;
}

bool Instrument_MapOld::KeyBuffered (DWORD key)
{
	switch (key) {
	case OAPI_KEY_K:  // track mode on/off
		ToggleTrack ();
		Refresh();
		return true;
	case OAPI_KEY_R:  // select reference
		OpenSelect_CelBody ("Map MFD: Reference", ClbkEnter_Map, 1);
		return true;
	case OAPI_KEY_T:  // select target
		g_select->Open ("Map MFD: Target", ClbkSubmn_Target, ClbkEnter_Target, (void*)this);
		return true;
	case OAPI_KEY_Z:  // zoom in/out
		SetZoom (!zoom);
		Refresh();
		return true;
	}
	return false;
}

bool Instrument_MapOld::KeyImmediate (char *kstate)
{
	if (KEYDOWN (kstate, OAPI_KEY_LBRACKET)) { // scroll left
		if (!track && BufKey (OAPI_KEY_LBRACKET, 0.05)) {
			if ((mapx -= 1) < 0) mapx += mapw;
			Refresh();
		}
		return true;
	}
	if (KEYDOWN (kstate, OAPI_KEY_RBRACKET)) { // scroll left
		if (!track && BufKey (OAPI_KEY_RBRACKET, 0.05)) {
			if ((mapx += 1) > mapw-IW) mapx -= mapw;
			Refresh();
		}
		return true;
	}
	if (KEYDOWN (kstate, OAPI_KEY_MINUS)) { // scroll down
		if (!track && BufKey (OAPI_KEY_MINUS, 0.05)) {
			if ((mapy -= 1) < 0) mapy = 0;
			Refresh();
		}
		return true;
	}
	if (KEYDOWN (kstate, OAPI_KEY_EQUALS)) { // scroll down
		if (!track && BufKey (OAPI_KEY_EQUALS, 0.05)) {
			if ((mapy += 1) > maph-IW/2) mapy = maph-IW/2;
			Refresh();
		}
		return true;
	}
	return false;
}

bool Instrument_MapOld::ProcessButton (int bt, int event)
{
	static const DWORD btkey[8] = { OAPI_KEY_R, OAPI_KEY_T, OAPI_KEY_K, OAPI_KEY_Z, OAPI_KEY_LBRACKET, OAPI_KEY_RBRACKET, OAPI_KEY_MINUS, OAPI_KEY_EQUALS };
	if (event & PANEL_MOUSE_LBDOWN) {
		if (bt < 4) return KeyBuffered (btkey[bt]);
	} else if (event & PANEL_MOUSE_LBPRESSED) {
		if (bt >= 4 && bt < 8) return KeyImmediate (KstateSet (btkey[bt]));
	}
	return false;
}

const char *Instrument_MapOld::BtnLabel (int bt) const
{
	static const char *label[8] = { "REF", "TGT", "TRK", "ZM", "<<", ">>", "UP", "DN" };
	return (bt < 8 ? label[bt] : 0);
}

int Instrument_MapOld::BtnMenu (const MFDBUTTONMENU **menu) const
{
	static const MFDBUTTONMENU mnu[8] = {
		{"Orbit reference", 0, 'R'},
		{"Select target", 0, 'T'},
		{"Track mode on/off", 0, 'K'},
		{"Zoom in/out", 0, 'Z'},
		{"Scroll left", 0, '['},
		{"Scroll right", 0, ']'},
		{"Scroll up", 0, '-'},
		{"Scroll down", 0, '='}
	};
	if (menu) *menu = mnu;
	return 8;
}

void Instrument_MapOld::UpdateMap ()
{
	static oapi::IVECTOR2 p[INSTRMAP_NPROJPT+4], pi0, pi1;
	bool have_shppos = false;

	gc->clbkBlt (map, 0, 0, bgmap); // copy background
	if (!refplanet) return;
	const SurfParam *sp = vessel->GetSurfParam();
	const Elements *el = vessel->Els();

	oapi::Sketchpad *skp = gc->clbkGetSketchpad(map);
	if (skp) {
		oapi::Pen *ppen;
		if (btgt) {  // mark target base in map
			ppen = skp->SetPen (draw[1][0].solidpen);
			skp->Ellipse (tgtx-3, tgty-3, tgtx+4, tgty+4);
			skp->SetPen (ppen);
		}
		if (otgt) {  // draw orbit projection
			const Elements *tel = otgt->Els();
			if (tel && otgt->ElRef() == refplanet) {
				double lng, lat, r;
				int xo, yo;
				CalcOrbitProj (tel, refplanet, p);
				ppen = skp->SetPen (draw[1][0].solidpen);
				skp->Polyline (p, INSTRMAP_NPROJPT+2);
				if (CalcIntersect (tel, refplanet, refplanet->Size(), p+0, p+1)) {
					skp->Rectangle (p[0].x-4, p[0].y-4, p[0].x+5, p[0].y+5);
					skp->SetBrush (brush[1]);
					skp->Rectangle (p[1].x-4, p[1].y-4, p[1].x+5, p[1].y+5);
					skp->SetBrush (0);
				}
				refplanet->GlobalToEquatorial (otgt->GPos(), lng, lat, r);
				CalcCoords (lng, lat, xo, yo);
				skp->Line (xo, yo-4, xo, yo+5);
				skp->Line (xo-4, yo, xo+5, yo);
				skp->SetPen (ppen);
			}
		}
		if (sp && vessel->ProxyPlanet() == refplanet) {
			CalcCoords (sp->lng, sp->lat, shpx, shpy);
			have_shppos = true;
		}
		if (el && vessel->ElRef() == refplanet) {
			// draw ship's orbital plane
			CalcOrbitProj (el, refplanet, p);
			ppen = skp->SetPen (draw[0][0].solidpen);
			skp->Polyline (p, INSTRMAP_NPROJPT+2);
			if (have_shppos && CalcIntersect (el, refplanet, refplanet->Size(), &pi0, &pi1)) {
				skp->SetPen (draw[3][0].solidpen);
				int i, j;
				if (p[INSTRMAP_NPROJPT].x-p[0].x >= 0) { // prograde motion
					if (pi1.x > pi0.x) {
						for (i = 0; i < INSTRMAP_NPROJPT+1 && p[i+1].x <= pi0.x; i++);
						for (j = INSTRMAP_NPROJPT+1; j > 0 && p[j-1].x >= pi1.x; j--);
						skp->Polyline (p, i+1);
						skp->Line (p[i].x, p[i].y, pi0.x, pi0.y);
						skp->Polyline (p+j, INSTRMAP_NPROJPT+2-j);
						skp->Line (pi1.x, pi1.y, p[j].x, p[j].y);
					} else if (pi1.x < pi0.x) {
						for (i = 0; i < INSTRMAP_NPROJPT+1 && p[i].x <= pi1.x; i++);
						for (j = i+1; j < INSTRMAP_NPROJPT+1 && p[j+1].x < pi0.x; j++);
						skp->Line (pi1.x, pi1.y, p[i].x, p[i].y);
						skp->Polyline (p+i, j-i+1);
						skp->Line (p[j].x, p[j].y, pi0.x, pi0.y);
					} else { // x values of intersections equal: just check completely inside/outside
						if (0.5*(el->ApDist() + el->PeDist()) < refplanet->Size()) // inside
							skp->Polyline (p, INSTRMAP_NPROJPT+2);
					}
				} else { // retrograde
					if (pi1.x < pi0.x) {
						for (i = 0; i < INSTRMAP_NPROJPT+1 && p[i+1].x >= pi0.x; i++);
						skp->Polyline (p, i+1);
						skp->Line (p[i].x, p[i].y, pi0.x, pi0.y);
						for (j = INSTRMAP_NPROJPT+1; j > 0 && p[j-1].x <= pi1.x; j--);
						skp->Polyline (p+j, INSTRMAP_NPROJPT+2-j);
						skp->Line (pi1.x, pi1.y, p[j].x, p[j].y);
					} else if (pi1.x > pi0.x) {
						for (i = 0; i < INSTRMAP_NPROJPT+1 && p[i].x >= pi1.x; i++);
						for (j = i+1; j < INSTRMAP_NPROJPT+1 && p[j+1].x >= pi0.x; j++);
						skp->Polyline (p+i, j-i+1);
						skp->Line (pi1.x, pi1.y, p[i].x, p[i].y);
						skp->Line (p[j].x, p[j].y, pi0.x, pi0.y);
					} else { // x values of intersections equal: just check completely inside/outside
						if (0.5*(el->ApDist() + el->PeDist()) < refplanet->Size()) // inside
							skp->Polyline (p, INSTRMAP_NPROJPT+2);
					}
				}

				skp->Rectangle (pi1.x-2, pi1.y-2, pi1.x+3, pi1.y+3);
				skp->Rectangle (pi1.x-1, pi1.y-1, pi1.x+2, pi1.y+2);
				skp->SetPen (draw[0][0].solidpen);
				skp->Rectangle (pi0.x-2, pi0.y-2, pi0.x+3, pi0.y+3);
				skp->Rectangle (pi0.x-1, pi0.y-1, pi0.x+2, pi0.y+2);
			}
			skp->SetPen (ppen);
		}
		if (have_shppos) {
			// mark ship's location
			skp->SetPen (draw[2][0].solidpen);
			skp->Line (shpx, shpy-4, shpx, shpy+5);
			skp->Line (shpx-4, shpy, shpx+5, shpy);
		}
		gc->clbkReleaseSketchpad (skp);
	}
}

void Instrument_MapOld::UpdateBlt ()
{
	if (needmap) {
		LoadMap (refplanet);
		CalcTargetCoords();
		needmap = false;
	}
	if (map) {
		UpdateMap ();
		if (track) {
			mapx = shpx - IW/2;
			mapy = min (maph-IW/2, (max (0, shpy - IW/4)));
		}
		RECT r = {mapx, mapy, mapx+IW, mapy+IW/2};
		if (mapx < 0) {
			r.left = mapw+mapx; r.right = mapw;
			gc->clbkBlt (surf, 0, IH-IW/2, map, mapw+mapx, mapy, -mapx, IW/2);
			r.left = 0; r.right = IW+mapx;
			if (r.right > 0)
				gc->clbkBlt (surf, -mapx, IH-IW/2, map, 0, mapy, IW+mapx, IW/2);
		} else if (mapx+IW > mapw) {
			r.right = mapw;
			gc->clbkBlt (surf, 0, IH-IW/2, map, mapx, mapy, mapw-mapx, IW/2);
			r.left = 0; r.right = IW-mapw+mapx;
			gc->clbkBlt (surf, mapw-mapx, IH-IW/2, map, 0, mapy, IW-mapw+mapx, IW/2);
		} else {
			gc->clbkBlt (surf, 0, IH-IW/2, map, mapx, mapy, IW, IW/2);
		}
	}
}

void Instrument_MapOld::UpdateDraw (oapi::Sketchpad *skp)
{
	//static POINT p[INSTRMAP_NPROJPT+4], pi0, pi1;

	if (refplanet) {

		int x1 = cw/2, x2 = IW/2, dy = ch, y = dy/2;
		double blng, blat, adist, hdg, rad;
		char cbuf[40];
		const SurfParam *sp = vessel->GetSurfParam();
		const Elements *el = vessel->Els();

		skp->SetTextColor (draw[0][0].col);
		if (btgt) {
			// output base data
			strcpy (datastr[0]+13, btgt->Name());
			skp->Text (x1, y+dy, datastr[0], strlen(datastr[0]));
			btgt->EquPos (blng, blat);
