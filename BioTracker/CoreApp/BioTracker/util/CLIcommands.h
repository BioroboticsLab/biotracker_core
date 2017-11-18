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
#include "settings/Settings.h"
#include "util/types.h"

class CLI {
public:
	static int optionParser(int ac, char* av[])
	{

		BioTracker::Core::Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);

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
				;

			options_description gui("GUI options");
			//gui.add_options()
			//	("display", value<std::string>(), "display to use")
			//	;

			// Declare an options description instance which will include
			// all the options
			options_description all("Allowed options");
			all.add(general).add(gui);

			// Declare an options description instance which will be shown
			// to the user
			options_description visible("Allowed options");
			visible.add(general).add(gui);


			variables_map vm;
			store(parse_command_line(ac, av, all), vm);

			if (vm.count("help")) {
				std::cout << visible;
				exit(0);
			}
			if (vm.count("usePlugin")) {
				const std::string& s = vm["usePlugin"].as<std::string>();
				std::string *usePlugins = new std::string(s);
				set->storeValue("usePlugins", (void*)usePlugins);
			}
			if (vm.count("video")) {
				const std::string& s = vm["video"].as<std::string>();
				std::string *video = new std::string(s);
				set->storeValue("video", (void*)video);
			}
		}
		catch (std::exception& e) {
			std::cout << e.what() << "\n";
		}
	}
};
