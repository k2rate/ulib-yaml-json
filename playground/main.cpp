#include <ulib/yaml_json.h>
#include <futile/futile.h>
#include <fmt/format.h>

int main()
{
    try
    {
        ulib::json value = ulib::parse_yaml_json(futile::open("test.yml").read());
        fmt::print("{}", value.dump());
    }
    catch (const std::exception &ex)
    {
        fmt::print("exception: {}\n", ex.what());
    }

    return 0;
}
