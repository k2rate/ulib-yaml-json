#include "yaml_json.h"
#include <yaml-cpp/yaml.h>

namespace ulib
{
    namespace detail
    {
        json convert_scalar(const YAML::Node &node)
        {
            if (bool bool_value; YAML::convert<bool>::decode(node, bool_value))
            {
                return json{bool_value};
            }

            if (int64_t int_value; YAML::convert<int64_t>::decode(node, int_value))
            {
                return json{int_value};
            }

            if (double double_value; YAML::convert<double>::decode(node, double_value))
            {
                return json{double_value};
            }

            return json{node.Scalar()};
        }

        json convert_node(const YAML::Node &node);
        json convert_map(const YAML::Node &node)
        {
            json value;
            for (auto it = node.begin(); it != node.end(); it++)
            {
                value[it->first.as<std::string>()] = convert_node(it->second);
            }

            return value;
        }

        json convert_sequence(const YAML::Node &node)
        {
            json value;
            for (auto it = node.begin(); it != node.end(); it++)
            {
                value.push_back() = convert_node(*it);
            }

            return value;
        }

        json convert_node(const YAML::Node &node)
        {
            switch (node.Type())
            {
            case YAML::NodeType::Map:
                return convert_map(node);
            case YAML::NodeType::Sequence:
                return convert_sequence(node);
            case YAML::NodeType::Scalar:
                return detail::convert_scalar(node);
            case YAML::NodeType::Null:
                return json{};
            case YAML::NodeType::Undefined:
                return json{};
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
        return detail::convert_node(node);
    }
} // namespace ulib