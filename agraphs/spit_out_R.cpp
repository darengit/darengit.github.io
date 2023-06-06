#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

struct AGraphComponent {
	bool validity = false;
	bool hedged = false;

	std::string qName;
	std::string rootName;


	std::vector<std::string> split(const std::string &s, char delim) {
    	std::vector<std::string> result;
    	std::stringstream ss (s);
    	std::string item;

    	for(;std::getline(ss, item, delim);)
        	result.push_back(item);

    	return result;
	}

	std::string join(const std::vector<std::string>& v, char delim) {
		std::string result;
		for(auto& str: v) result += str + delim;
		result.pop_back();
		return result;
	}

	AGraphComponent(const std::string& fn) {
		if(fn.size() < 10) return;
		std::string extension = fn.substr(fn.size()-4);
		if(extension != ".csv") return;

		auto splitName = split(fn.substr(0, fn.size()-4), '_');
		if(splitName.size() < 3) return;

		auto& qStr = splitName.back();
		if(qStr.size()!=2 || qStr[0]!='Q') return;

		auto& yearStr = splitName[splitName.size()-2];
		if(yearStr.size()!=4 || yearStr.substr(0,2)!="20") return;

		qName = join({yearStr, qStr}, '_');

		splitName.pop_back();
		splitName.pop_back();

		if(splitName.back() == "HEDGED") {
			hedged = true;
			splitName.pop_back();
		}

		rootName = join(splitName, '_');
		validity = true;
	}

	bool valid() {
		return validity;
	}

	std::string root() {
		return rootName;
	}

	std::string componentName() {
		std::string compName = rootName;
		if(hedged) compName += "_HEDGED";
		compName += '_'+qName;
		return compName;
	}

	std::string generateR() {
		std::string componentR = componentName();
		componentR += " <- read.csv(\"./csv/" + componentName() + ".csv\")";
		return componentR;
	}

	bool operator<(const AGraphComponent& other) {
		return qName < other.qName;
	}
};

struct AGraphSeries {
	std::vector<AGraphComponent> components;

	std::string generateR() {
		auto rootName = components[0].rootName;
		std::string result = "g <- plotHist(";
		for(int i=0; i<components.size() && i<5; ++i)
			result += components[i].componentName() + " ,";
		result.pop_back();
		result.pop_back();

		result += ")\n";
		result += "saveWidget(g, \"./html/"+rootName+".html\", title=\""+rootName+"\")";
		
		result += "\n";
		result += "print(\""+rootName+"\", quote=FALSE)";
		return result;
	}
};

int main() {
	std::string path = "./csv";

	std::unordered_map<std::string, AGraphSeries> agSeries;

	for(const auto& agraphFile: std::filesystem::directory_iterator(path)) {
		std::string fn = agraphFile.path().filename();

		AGraphComponent agc = AGraphComponent(fn);
		if(!agc.valid()) {
			std::cout << "invalid filename " << fn << std::endl;
			continue;
		}

		//if(agSeries.count(agc.root())) {
			auto& comps = agSeries[agc.root()].components;
			comps.push_back(agc);
			std::sort(comps.rbegin(), comps.rend());
		//} else {
		//	agSeries[agc.root()] = AGraphSeries(agc);
		//}
		
		std::ofstream readCsvR;
		std::ofstream saveWidgetR;

		readCsvR.open("readCsv.R");
		saveWidgetR.open("saveWidget.R");

		for(auto& pr: agSeries) {
			for(auto& component: pr.second.components)
				readCsvR << component.generateR() << std::endl;
			saveWidgetR << pr.second.generateR() << std::endl;
		}	

		readCsvR.close();
		saveWidgetR.close();

	}
}
