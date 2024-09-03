#include "yaml_json.h"
#include <yaml-cpp/yaml.h>

namespace ulib
{
    namespace detail
    {
        void convert_scalar(json &dest, const YAML::Node &node)
        {
            if (bool bool_value; YAML::convert<bool>::decode(node, bool_value))
            {
                dest = json{bool_value};
                return;
            }

            if (int64_t int_value; YAML::convert<int64_t>::decode(node, int_value))
            {
                dest = json{int_value};
                return;
            }

            if (double double_value; YAML::convert<double>::decode(node, double_value))
            {
                dest = json{double_value};
                return;
            }

            dest = json{node.Scalar()};
        }

        void convert_node(json &dest, const YAML::Node &node);
        void convert_map(json &dest, const YAML::Node &node)
        {
            for (auto it = node.begin(); it != node.end(); it++)
            {
                auto &field = dest[it->first.as<std::string>()];
                convert_node(field, it->second);
            }
        }

        void convert_sequence(json &dest, const YAML::Node &node)
        {
            for (auto it = node.begin(); it != node.end(); it++)
            {
                convert_node(dest.push_back(), *it);
            }
        }

        void convert_node(json &dest, const YAML::Node &node)
        {
            switch (node.Type())
            {
            case YAML::NodeType::Map:
                convert_map(dest, node);
                return;
            case YAML::NodeType::Sequence:
                convert_sequence(dest, node);
                return;
            case YAML::NodeType::Scalar:
                detail::convert_scalar(dest, node);
                return;
            case YAML::NodeType::Null:
                dest = json{};
                return;
            case YAML::NodeType::Undefined:
                dest = json{};
                return;
            default:
                throw ulib::json::exception{"Invalid YAML NodeType"};
            }
        }
    } // namespace detail

    json parse_yaml_json(string_view str)
    {
        auto data = ulib::str(str);
        while (data.ends_with(0)) // it can be more than 0
            data.pop_back();
        // data.MarkZeroEnd();

        YAML::Node node = YAML::Load(data);
        json value;
        detail::convert_node(value, node);
        return value;
    }
} // namespace ulib