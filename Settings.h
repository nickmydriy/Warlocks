#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

class Settings {
    private:
		std::map<std::string, std::string> settingsMap;
		std::string fileName;
		void save();
		void checkFile();
	public:
		/**
		 * Construct settings store
		 * and load data from file (if exists)
		 * \param filename Path to file with settings
		 */
		Settings(std::string const &fileName);
		/**
		 * Get setting value
		 * \param name Setting unique identifier
		 * \param def Default setting value
		 * \return Stored value for given name or default value
		 */
		std::string const &get(std::string const &name,
			std::string const &def = "") const;
		/**
		 * Set or replace setting value and save changes to file
		 * \param name Setting unique identifier
		 * \param value New setting value
		 */
		void set(std::string const &name,
			std::string const &value);
		/**
		 * Reset all settings
		 */
		void reset();
		/**
		 * Reload all settings from file
		 */
		void reload();
};