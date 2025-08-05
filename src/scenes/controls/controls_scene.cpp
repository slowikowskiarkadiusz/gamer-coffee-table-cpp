#import "controls_scene.hpp"

std::map<std::string, std::vector<controls_data> > controls_scene::controls = {
    {
        "pong_scene",
        {
            {{key::P1_LEFT}, "move left"},
            {{key::P1_RIGHT}, "move right"},
        }
    },
    {
        "tetris_scene", {
            {
                {{key::P1_LEFT}, "move left"},
                {{key::P1_RIGHT}, "move right"},
                {{key::P1_DOWN}, "fall"},
                {{key::P1_UP}, "drop"},
                {{key::P1_BLUE}, "rotate left"},
                {{key::P1_GREEN}, "rotate left"},
                {{key::P1_GREEN, key::P1_BLUE}, "swap", "+"},
            },
        }
    },
    {
        "tanks_scene", {
            {
                {{key::P1_ANY_DIRECTION}, "move"},
                {{key::P1_GREEN}, "fire"},
                {{key::P1_BLUE}, "fire"},
            },
        }
    }
};
