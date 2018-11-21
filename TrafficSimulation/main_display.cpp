#include "pch.h"
#include "main_display.h"

#include "entity.h"
#include "grid.h"
#include "display.h"
#include "editor.h"
#include "graph.h"
#include "query.h"

#include <vector>

void MainDisplay::init()
{
    display = new Display;
    display->init("TrafficSimulation", 1400, 800);
    display->shown = true;
}

void MainDisplay::draw()
{
    SDL_RenderClear(display->ren);
    SDL_SetRenderDrawBlendMode(display->ren, SDL_BLENDMODE_BLEND);

    grid->draw(display->ren);
    entity_system->draw(display->ren);
    query->highlight_accordingly(display->ren);
    gp->draw_the_path(display->ren, v_grid);
    v_grid->draw(display->ren);
	 ed->check_and_highlight_vehicle_indices(display->ren);
	 ed->check_and_highlight_bus(display->ren);
	 ed->check_and_highlight_pns_and_des_bst(display->ren);

    SDL_SetRenderDrawColor(display->ren, 150, 150, 150, 255);
    SDL_RenderPresent(display->ren);
}

void MainDisplay::destroy()
{
    display->destroy();
}
