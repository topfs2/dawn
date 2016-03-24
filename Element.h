#pragma once
#include "types.h"

namespace dawn
{
    class Component;

    typedef Component *(*ComponentFactory)(const std::string &type, const PropertyMap &props);

    class ComponentConstructor
    {
    public:
        ComponentConstructor(ComponentFactory factory, const std::string &type) : factory(factory), type(type) { }

        const ComponentFactory factory;
        const std::string type;
    };

    class Element
    {
    public:
        Element(const std::string &type, const PropertyMap &props, const std::vector<Element> &children) : m_factory(NULL), m_type(type), props(props), children(children) { }
        Element(const std::string &type, const PropertyMap &props, const Element &child) : m_factory(NULL), m_type(type), props(props), children({ child })  { }
        Element(const std::string &type, const PropertyMap &props) : m_factory(NULL), m_type(type), props(props) { }
        Element(const std::string &type) : m_factory(NULL), m_type(type)  { }

        Element(const ComponentConstructor &constructor, const PropertyMap &props, const std::vector<Element> &children) : m_factory(constructor.factory), m_type(constructor.type), props(props), children(children) { }
        Element(const ComponentConstructor &constructor, const PropertyMap &props, const Element &child) : m_factory(constructor.factory), m_type(constructor.type), props(props), children({ child }) { }
        Element(const ComponentConstructor &constructor, const PropertyMap &props) : m_factory(constructor.factory), m_type(constructor.type), props(props) { }
        Element(const ComponentConstructor &constructor) : m_factory(constructor.factory), m_type(constructor.type) { }

        virtual ~Element() { }

        std::string type() const {
            return m_type;
        }

        bool is(const std::string &other) const {
            return m_type == other && m_factory == NULL;
        }

        bool sameAs(const Element &other) const {
            return m_factory == other.m_factory && m_type == other.m_type;
        }

        bool has(const std::string &prop) const {
            return props.find(prop) != props.end();
        }

        template<typename T>
        T get(const std::string &prop, T fallback) const {
            PropertyMap::const_iterator itr = props.find(prop);
            if (itr != props.end()) {
                boost::any value = itr->second;
                if (value.type() == typeid(T)) {
                    return boost::any_cast<T>(value);
                }
            }

            return fallback;
        }

        Component *construct() const {
            if (m_factory) {
                return m_factory(m_type, props);
            }

            return NULL;
        }

    private:
        const ComponentFactory m_factory;
        const std::string m_type;

    public:
        const PropertyMap props;
        const std::vector<Element> children;
    };

    typedef std::vector<Element> ChildVector;
}
