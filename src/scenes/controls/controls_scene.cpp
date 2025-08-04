#import "controls_scene.hpp"

std::map<std::string, std::vector<controls_data> > controls_scene::controls = {
    {
        "pong_scene",
        {{{key::P1_L, key::P1_R}, "move"}}
    },
    {
        "tetris_scene", {
            {{key::P1_L, key::P1_R}, "move"},
            {{key::P1_L, key::P1_R}, "fall", "+"},
            {{key::P1_L_BLUE}, "drop"},
            {{key::P1_R_GREEN}, "swap"}
        }
    },
    {
        "tanks_scene", {
            {{key::P1_L}, "rotate left"},
            {{key::P1_R}, "rotate right"},
            {{key::P1_L, key::P1_R}, "move forward", "+"},
            {{key::P1_L_BLUE}, "fire"},
            {{key::P1_R_GREEN}, "fire"}
        }
    }
};
