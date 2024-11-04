#pragma once

#include "CanvasObject.hpp"
#include "RoomState.hpp"
#include <list>
#include <mutex>

class Selection
{
public:
    std::string owner_id;
    std::string room_id;
    uint64_t page_id;

    std::list<uint64_t> sel_list;

    void add_selection (std::list<uint64_t> obj_ids)
    {
        sel_list.splice(sel_list.end(), obj_ids);
    }
    void add_selection (uint64_t obj_id)
    {
        sel_list.push_back(obj_id);
    }

    void delete_selection ()
    {
        for (std::list<uint64_t>::iterator it=sel_list.begin(); it != sel_list.end(); ++it)
        {
            uint64_t del_obj_id = *it;
            state.manipulatePage(page_id, [del_obj_id](Page& page)
            {
                page.deleteObject(del_obj_id);
            });
        }
        clear_selection();
    }

    void clear_selection()
    {
        sel_list.clear();
    }



    // Constructor
    Selection(const std::string &room_id) : room_id(room_id) {};

private:

};

#ifndef SELECTION_HPP
#define SELECTION_HPP
#endif // SELECTION_HPP
