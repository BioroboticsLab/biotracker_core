#pragma once



#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

#include <iostream>
#include <fstream>
#include <exception>

#include <qfile.h>
#include <qfileinfo.h>
#include "util/types.h"
#include "util/Config.h"

class CLI {
public:
	static void optionParser(int ac, char* av[], Config* cfg)
	{
		//Taken from http://www.boost.org/doc/libs/1_64_0/libs/program_options/example/option_groups.cpp
		using namespace boost;
		using namespace boost::program_options;
		try {
			// Declare three groups of options.
			options_description general("General options");
			general.add_options()
				("help", "Produce this help message")
				("usePlugin", value<std::string>(), "Uses plugin from given filepath")
				("video", value<std::string>(), "Loads a video from given filepath")
				("cfg", value<std::string>(), "Provide custom path to a config file")
				;

			options_description gui("GUI options");
			//gui.add_options()
			//	("display", value<std::string>(), "display to use")
			//	;

			// For "Open with ..."
			positional_options_description unnamed;
			unnamed.add("video", 1);

			// Declare an options description instance which will include
			// all the options
			options_description all("Allowed options");
			all.add(general).add(gui);

			// Declare an options description instance which will be shown
			// to the user
			options_description visible("Allowed options");
			visible.add(general).add(gui);


			variables_map vm;
			store(command_line_parser(ac, av).options(all).positional(unnamed).run(), vm);

			if (vm.count("help")) {
				std::cout << visible;
				exit(0);
			}
			if (vm.count("usePlugin")) {
				auto str = vm["usePlugin"].as<std::string>();
				cfg->UsePlugins = QString(str.c_str());
			}
			if (vm.count("video")) {
				auto str = vm["video"].as<std::string>();
				cfg->LoadVideo = QString(str.c_str());
			}
			if (vm.count("cfg")) {
				auto str = vm["cfg"].as<std::string>();
				cfg->CfgCustomLocation = QString(str.c_str());
			}
		}
		catch (std::exception& e) {
			std::cout << e.what() << "\n";
		}
	}
};
