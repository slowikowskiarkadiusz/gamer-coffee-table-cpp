//
// Created by Arkadiusz on 27/07/2025.
//

#pragma once

#include "../../scene.hpp"

#include <map>
#include <string>
#include <memory>

class menu_scene : public scene<std::nullptr_t> {
    menu_scene(): scene<std::nullptr_t>(nullptr) {
        std::cout << "huj" << std::endl;
    }
};
