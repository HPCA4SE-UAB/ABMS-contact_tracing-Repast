/* Model.h */
/* 
* Benchmark model for Repast HPC ABMS
* This file is part of the ABMS-Benchmark-FLAME distribution (https://github.com/xxxx).
* Copyright (c) 2018 Universitat Autònoma de Barcelona, Escola Universitària Salesiana de Sarrià
* 
*Based on: Alban Rousset, Bénédicte Herrmann, Christophe Lang, Laurent Philippe
*A survey on parallel and distributed multi-agent systems for high performance comput-
*ing simulations Computer Science Review 22 (2016) 27–46
*
* This program is free software: you can redistribute it and/or modify  
* it under the terms of the GNU General Public License as published by  
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
* 
*  You should have received a copy of the GNU General Public License 
*  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef MODEL
#define MODEL

#include <boost/mpi.hpp>
#include "repast_hpc/Schedule.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/AgentRequest.h"
#include "repast_hpc/TDataSource.h"
#include "repast_hpc/SVDataSet.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/GridComponents.h"

#include "ogrsf_frmts.h"

#include "Agent.h"

#include <string>


/* Agent Package Provider */
class RepastHPCAgentPackageProvider {
	
private:
    repast::SharedContext<RepastHPCAgent>* agents;
	
public:
	
    RepastHPCAgentPackageProvider(repast::SharedContext<RepastHPCAgent>* agentPtr);
	
    void providePackage(RepastHPCAgent * agent, std::vector<RepastHPCAgentPackage>& out);
	
    void provideContent(repast::AgentRequest req, std::vector<RepastHPCAgentPackage>& out);
	
};

/* Agent Package Receiver */
class RepastHPCAgentPackageReceiver {
	
private:
    repast::SharedContext<RepastHPCAgent>* agents;
	RepastHPCModel* _model;
	
public:
	
    RepastHPCAgentPackageReceiver(repast::SharedContext<RepastHPCAgent>* agentPtr, RepastHPCModel* model);
	
    RepastHPCAgent * createAgent(RepastHPCAgentPackage package);
	
    void updateAgent(RepastHPCAgentPackage package);
	
};


/* Data Collection */
class DataSource_AgentsInfected: public repast::TDataSource<int>{
private:
	repast::SharedContext<RepastHPCAgent>* context;

public:
	DataSource_AgentsInfected(repast::SharedContext<RepastHPCAgent>* c);
	int getData();
};
	

class DataSource_AgentsSick: public repast::TDataSource<int>{
private:
	repast::SharedContext<RepastHPCAgent>* context;

public:
	DataSource_AgentsSick(repast::SharedContext<RepastHPCAgent>* c);
	int getData();
};


class DataSource_AgentNumber : public repast::TDataSource<int>{
private:
	repast::SharedContext<RepastHPCAgent>* context;
	RepastHPCModel* model;	
public:
	DataSource_AgentNumber(repast::SharedContext<RepastHPCAgent>* c, RepastHPCModel* m);
	int getData();
};


class RepastHPCModel{
	int stopAt;
	int countOfAgents;
	int procPerx;
	int procPery;
	int agentsToAdd;

	std::string initialGISFile;
	
	// World size props
	int WIDTH;
	int HEIGHT;
	OGRPolygon *poPolygon;
	OGRPolygon Polygon;

	// Walker props
	int walkerRate;
    float drifting;
    float stopping;
    int stopTime;
	float wander;
    int speed;
	float initialDensity;

	// Epidemiology props
	float infectiousness;
	int infectoinRadius;
	float sickRate;
	int encounterRadius;
	int maxSick;

	// Phone props
	int signalRadius;
	float applicationRate;
	int phoneThreshold1;
	int phoneThreshold2;

	std::vector<repast::Point<int>> topLimit;
	std::vector<repast::Point<int>> botLimit;

	repast::Properties* props;
	repast::SharedContext<RepastHPCAgent> context;
	
	RepastHPCAgentPackageProvider* provider;
	RepastHPCAgentPackageReceiver* receiver;

	repast::SVDataSet* agentValues;
    repast::SharedDiscreteSpace<RepastHPCAgent, repast::StrictBorders, repast::SimpleAdder<RepastHPCAgent> >* discreteSpace;
	
public:
	RepastHPCModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm);
	~RepastHPCModel();
	void init();
	void requestAgents();
	void cancelAgentRequests();
	void removeLocalAgents();
	void printAgentsPosition();
	void doSomething();
	void initSchedule(repast::ScheduleRunner& runner);
	void recordResults();
	bool insideWorld(std::vector<int> agentNewLoc);
	bool checkPositionEmpty(repast::Point<int> agentNewLoc);
	bool loadPolygon(repast::Point<double> *origin, repast::Point<double> *extent);
};




#endif
