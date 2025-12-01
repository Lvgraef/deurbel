#pragma once

namespace component {
    class UltrasoneInterface {
    public:
        virtual ~UltrasoneInterface() = default;

        virtual void init() = 0;
        virtual void update() const = 0;
        [[nodiscard]] virtual uint16_t getDistance() const = 0;
    };
}
