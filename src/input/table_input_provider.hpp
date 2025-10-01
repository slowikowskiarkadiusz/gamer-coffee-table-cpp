//
// Created by Arkadiusz on 30/09/2025.
//

#ifndef TABLE_INPUT_PROVIDER_H
#define TABLE_INPUT_PROVIDER_H
#include "input_provider.hpp"

class table_input_provider : public input_provider {
public:
    void update(float delta_time) override;
    void lateUpdate(float delta_time) override;
    bool is_key_down(key key) const override;
    bool is_any_key_down() const override;
    bool is_key_up(key key) const override;
    bool is_any_key_up() const override;
    bool is_key_press(key key) const override;
    bool is_any_key_press() const override;
    void clear() override;
};

#endif //TABLE_INPUT_PROVIDER_H
