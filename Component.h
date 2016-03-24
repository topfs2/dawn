#pragma once
#include "types.h"
#include "Element.h"

namespace dawn
{
    class Component
    {
    public:
        virtual ~Component() { }

        virtual void componentReceiveProps(const PropertyMap &props) = 0;

        virtual void componentWillMount() = 0;
        virtual Element render() = 0;
        virtual void componentDidMount() = 0;
    };
}
