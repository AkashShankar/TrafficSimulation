#ifndef MAIN_DISPLAY_H
#define MAIN_DISPLAY_H

struct Display;
struct EntitySystem;
struct Grid;
struct Editor;
struct VirtualGrid;
struct Graph;
struct Query;

struct MainDisplay
{
    Display *display = nullptr;
    EntitySystem *entity_system = nullptr; // set this manually
    Grid *grid = nullptr; // set this manually
    Editor *ed = nullptr; // set this manually
    VirtualGrid *v_grid = nullptr; // set this manually
    Graph *gp = nullptr; // set this manually
    Query *query = nullptr; // set this manually

    void init();
    void draw();
    void destroy();
};

#endif