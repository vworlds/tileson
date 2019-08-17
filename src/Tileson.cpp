//
// Created by robin on 31.07.2019.
//


#include "Tileson.h"

/*!
 * Parses Tiled json data by file
 * @param path path to file
 * @return parsed data as Map
 */
tson::Map tson::Tileson::parse(const fs::path &path)
{
    if(fs::exists(path) && fs::is_regular_file(path))
    {
        std::ifstream i(path.u8string());
        nlohmann::json json;
        try
        {
            i >> json;
        }
        catch(const nlohmann::json::parse_error &error)
        {
            std::string message = "Parse error: ";
            message += std::string(error.what());
			message += std::string("\n");
            return tson::Map {tson::Map::ParseStatus::ParseError, message};
        }
        return parseJson(json);
    }

    std::string msg = "File not found: ";
    msg += std::string(path.u8string());
    return tson::Map {tson::Map::ParseStatus::FileNotFound, msg};
}

/*!
 * Parses Tiled json data by memory
 * @param data The data to parse
 * @param size The size of the data to parse
 * @return parsed data as Map
 */
tson::Map tson::Tileson::parse(const void *data, size_t size)
{
    std::istringstream i;
    i.rdbuf()->pubsetbuf((char *)data, size);
    nlohmann::json json;
	try
	{
		i >> json;
	}
	catch (const nlohmann::json::parse_error& error)
	{
		std::string message = "Parse error: ";
		message += std::string(error.what());
		message += std::string("\n");
		return tson::Map{ tson::Map::ParseStatus::ParseError, message };
	}

    return parseJson(json);
}

/*!
 * Common parsing functionality for doing the json parsing
 * @param json Tiled json to parse
 * @return parsed data as Map
 */
tson::Map tson::Tileson::parseJson(const nlohmann::json &json)
{
    tson::Map map;
    if(map.parse(json))
        return map;

    return tson::Map {tson::Map::ParseStatus::MissingData, "Missing map data..."};
}
