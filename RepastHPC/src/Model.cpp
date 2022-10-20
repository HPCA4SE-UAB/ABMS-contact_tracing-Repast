/* Model.cpp */
/* 
* Contact tracing model
* Copyright (c) 2021 Universitat Autònoma de Barcelona, Escola Universitària Salesiana de Sarrià
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




#include <stdio.h>
#include <vector>
#include <boost/mpi.hpp>
#include "repast_hpc/AgentId.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Utilities.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/initialize_random.h"
#include "repast_hpc/SVDataSetBuilder.h"
#include "repast_hpc/Point.h"
#include "repast_hpc/MultipleOccupancy.h"


#include "Model.h"



/*
 *    Class: RepastHPCAgentPackageProvider  
 * Function: RepastHPCAgentPackageProvider
 * --------------------
 * RepastHPCAgentPackageProvider constructor
 * 
 * agentPtr: agent
 *
 * returns: -
 */
RepastHPCAgentPackageProvider::RepastHPCAgentPackageProvider(repast::SharedContext<RepastHPCAgent>* agentPtr): agents(agentPtr){ 
}

/*
 *    Class: RepastHPCAgentPackageProvider  
 * Function: providePackage
 * --------------------
 * Builds ans agent package and puts in a vector
 * 
 * agent: agent
 * out: vector of agents packaages
 *
 * returns: -
 */
void RepastHPCAgentPackageProvider::providePackage(RepastHPCAgent * agent, std::vector<RepastHPCAgentPackage>& out){
    repast::AgentId id = agent->getId();
    
    RepastHPCAgentPackage package(id.id(), id.startingRank(), id.agentType(), id.currentRank(), agent->getStopping(), agent->getStopTime(), agent->getStopCounter(), agent->getEntryTime(), agent->getDrifting(), agent->getSpeed(), agent->directionTop(), agent->getCountInfected(), agent->getInfectionTime(), agent->getPhoneActiveCount(), agent->getTimeInSimulation(), agent->getInfectiousness(), agent->getEncounterRadius(), agent->isSick(), agent->isInfected(), agent->getTimeSpentWithOthers() , agent->getThreshold1(), agent->getThreshold2(), agent->getHasApplication(), agent->getSignalRadius(), agent->getEncountersReal(), agent->getEncountersRecorded(), agent->getPhone()->getBuckets(), agent->getPhone()->getBucketsPositive());

    out.push_back(package);
}

/*
 *    Class: RepastHPCAgentPackageProvider  
 * Function: provideContent
 * --------------------
 * Puts a vector of agents into a vector of agent packages
 * 
 * req: vector of agents
 * out: vector of agents packaages
 *
 * returns: -
 */
void RepastHPCAgentPackageProvider::provideContent(repast::AgentRequest req, std::vector<RepastHPCAgentPackage>& out){
    std::vector<repast::AgentId> ids = req.requestedAgents();
    for(size_t i = 0; i < ids.size(); i++){
        providePackage(agents->getAgent(ids[i]), out);
    }
}

/*
 *    Class: RepastHPCAgentPackageReceiver
 * Function: RepastHPCAgentPackageReceiver 
 * --------------------
 * RepastHPCAgentPackageReceiver constructor
 * 
 * agentPtr: agent
 *
 * returns: -
 */
RepastHPCAgentPackageReceiver::RepastHPCAgentPackageReceiver(repast::SharedContext<RepastHPCAgent>* agentPtr, RepastHPCModel* model): agents(agentPtr), _model(model){
}

/*
 *    Class: RepastHPCAgentPackageReceiver
 * Function: RepastHPCAgentPackageReceiver 
 * --------------------
 * RepastHPCAgentPackageReceiver constructor
 * 
 * package: agent package
 *
 * returns: new agent
 */
RepastHPCAgent * RepastHPCAgentPackageReceiver::createAgent(RepastHPCAgentPackage package){
    repast::AgentId id(package._id, package._rank, package._type, package._currentRank);
	Phone* phone = new Phone(package._threshold1,package._threshold2,package._hasApplication,package._signalRadius);

	phone->setBuckets(package._buckets);

	phone->setBucketsPositive(package._bucketsSick);

    std::vector<std::pair<repast::AgentId,int>> _encountersReal;
    std::vector<std::pair<repast::AgentId,int>> _encountersRecorded;

    return new RepastHPCAgent(_model, id, package._stopping, package._stopTime, package._stopCounter, package._entryTime, package._drifting, package._speed, package._directionTop, package._countInfected, package._infectionTime, package._phoneActiveCount, package._timeInSimulation, package._infectiousness, package._encounterRadius, package._sick, package._infected, package._timeSpentWithOthers, package._threshold1, package._threshold2, package._hasApplication, package._signalRadius, _encountersReal, _encountersRecorded, phone); //TODO ens pondem estalviar _threshold1, _threshold2, _hasApplication, _signalRaius en aquest construtor???
}

/*
 *    Class: RepastHPCAgentPackageReceiver
 * Function: updateAgent 
 * --------------------
 * Update agent with data received from a package
 * 
 * package: agent package
 *
 * returns: new agent
 */
void RepastHPCAgentPackageReceiver::updateAgent(RepastHPCAgentPackage package){
    repast::AgentId id(package._id, package._rank, package._type);
    RepastHPCAgent * agent = agents->getAgent(id);

    std::vector<int> _buckets;
    std::vector<int> _bucketsSick;
    std::vector<std::pair<repast::AgentId,int>> _encountersReal;
    std::vector<std::pair<repast::AgentId,int>> _encountersRecorded;

	agent->update(package._currentRank, package._stopping, package._stopTime, package._stopCounter, package._entryTime, package._drifting, package._speed, package._directionTop, package._countInfected, package._infectionTime, package._phoneActiveCount, package._timeInSimulation, package._infectiousness, package._encounterRadius, package._sick, package._infected, package._timeSpentWithOthers, package._threshold1, package._threshold2, package._hasApplication, package._signalRadius, package._encountersReal, package._encountersRecorded, package._buckets, package._bucketsSick);
}


/*
 *    Class: DataSource_AgentsInfected
 * Function: DataSource_AgentsInfected
 * --------------------
 * DataSource_AgentsInfected constructor
 * 
 * c: context
 *
 * returns: -
 */
DataSource_AgentsInfected::DataSource_AgentsInfected(repast::SharedContext<RepastHPCAgent>* c) : context(c){ 
}

/*
 *    Class: DataSource_AgentsInfected
 * Function: getData 
 * --------------------
 * Get total sum of local agents infected
 * 
 * -: -
 *
 * returns: sum
 */
int DataSource_AgentsInfected::getData(){
	int sum = 0, s;
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iter    = context->localBegin();
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iterEnd = context->localEnd();
	while( iter != iterEnd) {
		if ((*iter)->isInfected()) sum++;	
		iter++;
	}
	return sum;
}





/*
 *    Class: DataSource_AgentsSick
 * Function: DataSource_AgentsSick
 * --------------------
 * DataSource_AgentsSick constructor
 * 
 * c: context
 *
 * returns: -
 */
DataSource_AgentsSick::DataSource_AgentsSick(repast::SharedContext<RepastHPCAgent>* c) : context(c){ 
}

/*
 *    Class: DataSource_AgentsSick
 * Function: getData 
 * --------------------
 * Get total sum of local agents Sick
 * 
 * -: -
 *
 * returns: sum
 */
int DataSource_AgentsSick::getData(){
	int sum = 0, s;
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iter    = context->localBegin();
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iterEnd = context->localEnd();
	while( iter != iterEnd) {
		if ((*iter)->isSick()) sum++;	
		iter++;
	}
	return sum;
}

/*
 *    Class: DataSource_AgentNumber
 * Function: DataSource_AgentNumber 
 * --------------------
 * DataSource_AgentNumber constructor
 * 
 * c: context
 *
 * returns: -
 */
DataSource_AgentNumber::DataSource_AgentNumber(repast::SharedContext<RepastHPCAgent>* c, RepastHPCModel* m) : context(c), model(m){ 
}


/*
 *    Class: DataSource_AgentNumber
 * Function: getData 
 * --------------------
 * Get total sum of local number of agents counter
 * 
 * -: -
 *
 * returns: sum
 */
int DataSource_AgentNumber::getData(){
	int sum = 0;
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iter    = context->localBegin();
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iterEnd = context->localEnd();
	while( iter != iterEnd) {
		sum++;
		iter++;
	}
	return sum;
}



/*
 *    Class: RepastHPCModel
 * Function: RepastHPCModel 
 * --------------------
 * RepastHPCModel constructor
 * 
 * propsFile: props file
 * argc:
 * argv:
 * comm: mpi communicator
 *
 * returns: -
 */
RepastHPCModel::RepastHPCModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm): context(comm){
	props = new repast::Properties(propsFile, argc, argv, comm);
	stopAt = repast::strToInt(props->getProperty("stop.at"));

	procPerx = repast::strToInt(props->getProperty("proc.per.x"));
	procPery = repast::strToInt(props->getProperty("proc.per.y"));

	encounterRadius = repast::strToInt(props->getProperty("encounter.radious"));

	initialGISFile = props->getProperty("initial.GIS.file");

	// Walker props
	walkerRate = repast::strToInt(props->getProperty("Walker.walkerRate"));
	drifting = (double) repast::strToDouble(props->getProperty("Walker.drifting"));
	stopping = (double) repast::strToDouble(props->getProperty("Walker.stopping"));
	stopTime = repast::strToInt(props->getProperty("Walker.stopTime"));
	speed = repast::strToInt(props->getProperty("Walker.speed"));
	initialDensity = (double) repast::strToDouble(props->getProperty("Walker.initialDensity"));

	// Epidemiology props
    infectiousness = (double) repast::strToDouble(props->getProperty("Epidemiology.infectiousness"));
	infectoinRadius = repast::strToInt(props->getProperty("Epidemiology.infectoinRadius"));
	sickRate = (double) repast::strToDouble(props->getProperty("Epidemiology.sickRate"));
	encounterRadius = repast::strToInt(props->getProperty("Epidemiology.encounterRadius"));

	// Phone props
	signalRadius = repast::strToInt(props->getProperty("Phone.signalRadius"));
	applicationRate = (double) repast::strToDouble(props->getProperty("Phone.applicationRate"));
	phoneThreshold1 = repast::strToInt(props->getProperty("Phone.phoneThreshold1"));
	phoneThreshold2 = repast::strToInt(props->getProperty("Phone.phoneThreshold2"));

	initializeRandom(*props, comm);
	if(repast::RepastProcess::instance()->rank() == 0) props->writeToSVFile("./output/record.csv");
	provider = new RepastHPCAgentPackageProvider(&context);
	receiver = new RepastHPCAgentPackageReceiver(&context, this);

	repast::Point<double> origin(0,0);
	repast::Point<double> extent(0,0);    
	loadPolygon(&origin, &extent); //TODO: controlar que ha anat bé
	repast::GridDimensions gd(origin, extent);
	WIDTH = origin[0]+extent[0];
	HEIGHT = origin[1]+extent[1];

	std::vector<int> processDims;
	processDims.push_back(procPerx);
	processDims.push_back(procPery); //Nº process = procPerx*procPery

	//TODO: This part only works with top and botton regions parallel to X axe
	for (int i = 0; i < WIDTH; i++) {
        botLimit.push_back(repast::Point<int>(origin[0]+i,origin[1] + 5));
        topLimit.push_back(repast::Point<int>(origin[0]+i,origin[1]+extent[1] - 5));
    }
    
	discreteSpace = new repast::SharedDiscreteSpace<RepastHPCAgent, repast::StrictBorders, repast::SimpleAdder<RepastHPCAgent> >("AgentDiscreteSpace", gd, processDims, 2, comm);
	
	std::cout << "RANK " << repast::RepastProcess::instance()->rank() << " BOUNDS: " << discreteSpace->bounds().origin() << " " << discreteSpace->bounds().extents() << std::endl;
    
   	context.addProjection(discreteSpace);
    
	// Data collection
	// Create the data set builder
	std::string fileOutputName("./output/agent_total_data.csv");
	repast::SVDataSetBuilder builder(fileOutputName.c_str(), ",", repast::RepastProcess::instance()->getScheduleRunner().schedule());
	
	// Create the individual data sets to be added to the builder
	DataSource_AgentsInfected* agentsInfected_DataSource = new DataSource_AgentsInfected(&context);
	builder.addDataSource(createSVDataSource("AgentsInfected", agentsInfected_DataSource, std::plus<int>()));

	DataSource_AgentsSick* agentsSick_DataSource = new DataSource_AgentsSick(&context);
	builder.addDataSource(createSVDataSource("AgentsSick", agentsSick_DataSource, std::plus<int>()));

	DataSource_AgentNumber* agentNumber_DataSource = new DataSource_AgentNumber(&context, this);
	builder.addDataSource(createSVDataSource("NumAgents", agentNumber_DataSource, std::plus<int>()));

	// Use the builder to create the data set
	agentValues = builder.createDataSet();
}

/*
 *    Class: RepastHPCModel
 * Function: ~RepastHPCModel 
 * --------------------
 * RepastHPCModel destructor
 * 
 * -: -
 *
 * returns: -
 */
RepastHPCModel::~RepastHPCModel(){
	delete props;
	delete provider;
	delete receiver;
	delete agentValues;
}

/*
 *    Class: RepastHPCModel
 * Function: init 
 * --------------------
 * Creation of agents
 * 
 * -: -
 *
 * returns: -
 */
void RepastHPCModel::init(){
	int rank = repast::RepastProcess::instance()->rank();
	int x,y,z;
	FILE *fp;
	u_int32_t idg;
	repast::Point<int> initialLocation(0,0);
	int maxPositionsToLookForward = 3, j = 0;//TODO maxPositionsToLookForward com a paràmetre?


	float xmin = discreteSpace->dimensions().origin().getX();
	float ymin = discreteSpace->dimensions().origin().getY();
	float xmax = discreteSpace->dimensions().origin().getX() + discreteSpace->dimensions().extents().getX();
	float ymax = discreteSpace->dimensions().origin().getY() + discreteSpace->dimensions().extents().getY();
	countOfAgents = 0;

	/*Initial creation of agents*/

	int initialAgents = discreteSpace->dimensions().extents().getX() * discreteSpace->dimensions().extents().getY() * initialDensity;

	for (int i = 0; i < initialAgents - 1; i++){
		bool _sick = false;
    	if (repast::Random::instance()->nextDouble() < sickRate) _sick = true;
		bool _infected = _sick; //To be sick means to be infected too
		float _drifting = repast::Random::instance()->nextDouble() * drifting;
		float _stopping = repast::Random::instance()->nextDouble() * stopping;
		int _stopTime = repast::Random::instance()->nextDouble() * stopTime;
		float _speed = 0.2 * (2 * repast::Random::instance()->nextDouble() - 1) + speed;
		bool _hasApp = repast::Random::instance()->nextDouble() < applicationRate;
		bool _directionTop;
		if (repast::Random::instance()->nextDouble() < 0.5) _directionTop = true;
		else _directionTop = false;
	
		//Get an empty position
		//TODO JJ: Aquest codi ha de produir un repast::Point<int> i modificar la funció checkPositionEmpty perque ho testeji
		j = 0;
		do{
			x = xmin + (xmax - xmin) * repast::Random::instance()->nextDouble();
			y = ymin + (ymax - ymin) * repast::Random::instance()->nextDouble();
        	//repast::Point<int> initialLocation(x,y);//TODO initialLocation desapareix quan surti de la clau?
			//initialLocation(x,y); //TODO Està bé això?????
			initialLocation[0] = x;
			initialLocation[1] = y;

			if (j >= maxPositionsToLookForward) break;
			j++;

        }while(!checkPositionEmpty(initialLocation));
		
		if (j >= maxPositionsToLookForward) continue; //Do not add agent

		repast::AgentId id(countOfAgents, rank, 0);
        id.currentRank(rank);

        RepastHPCAgent* agent = new RepastHPCAgent(this, id, _stopping, _stopTime, 0, repast::RepastProcess::instance()->getScheduleRunner().currentTick(), _drifting, _speed, _directionTop, 0, -1, 0, repast::RepastProcess::instance()->getScheduleRunner().currentTick(), infectiousness, encounterRadius, _sick, _infected, 0, phoneThreshold1, phoneThreshold2, _hasApp, signalRadius); //stopCounter=0, entryTime= tick, countInfected=0, infectionTime = -1, phoneActiveCount=0, timeSpentWithOthers = 0


		context.addAgent(agent);
		discreteSpace->moveTo(id, initialLocation);
		countOfAgents++;
	}
}


/*
 *    Class: RepastHPCModel
 * Function: printAgentsPosition 
 * --------------------
 * Print agents position
 * 
 * -: -
 *
 * returns: -
 */
void RepastHPCModel::printAgentsPosition(){
	int rank = repast::RepastProcess::instance()->rank();

        std::vector<RepastHPCAgent*> agents;
        //context.selectAgents(repast::SharedContext<RepastHPCAgent>::LOCAL, countOfAgents, agents);
        context.selectAgents(repast::SharedContext<RepastHPCAgent>::LOCAL, agents);
        std::vector<RepastHPCAgent*>::iterator it = agents.begin();
        while(it != agents.end()){
		std::vector<int> agentLoc;
		discreteSpace->getLocation((*it)->getId(), agentLoc);
		repast::Point<int> agentLocation(agentLoc);
		std::cout << "T(" << repast::RepastProcess::instance()->getScheduleRunner().currentTick() << ")" << "P(" << rank << ") " << (*it)->getId() << " " << agentLocation << " Sick: " << (*it)->isSick() << " Infected: " << (*it)->isInfected() << std::endl;
                it++;
	}
}

/*
 *    Class: RepastHPCModel
 * Function: doSomething
 * --------------------
 * Run agents in every simulation step
 * 
 * -: -
 *
 * returns: -
 */
void RepastHPCModel::doSomething(){
	int whichRank = 0; 
	bool dierequest = false, reproductionrequest = false;
	int i; // a counter
	int maxPositionsToLookForward = 0, j = 0;//TODO maxPositionsToLookForward com a paràmetre?

	int rank = repast::RepastProcess::instance()->rank();

	if(rank == whichRank) std::cout << " TICK " << repast::RepastProcess::instance()->getScheduleRunner().currentTick() << std::endl;
	
	std::vector<RepastHPCAgent*> agents;
	context.selectAgents(repast::SharedContext<RepastHPCAgent>::LOCAL, agents);

std::cout << "# Agents in rank " << rank << " : " << agents.size() << std::endl;
	if (agents.size() == 0) return;

	// Infect
	std::vector<RepastHPCAgent*>::iterator it = agents.begin();
	while(it != agents.end()){
        	//std::cout << "Agent that tries to infect: " << (*it)->getId() << std::endl;
		(*it)->infect(&context, discreteSpace);
		it++;
	}


	// Move
	it = agents.begin();
	while(it != agents.end()){
		bool die = (*it)->move(discreteSpace);
		if (die){
        	repast::AgentId id = (*it)->getId();
            std::cout << "Agent to die: " << id << std::endl;
            repast::RepastProcess::instance()->agentRemoved(id);
            context.removeAgent(id);
		}

		it++;
	}


	// New agents
	float xmin = discreteSpace->dimensions().origin().getX();
	float ymin = discreteSpace->dimensions().origin().getY();
	float xmax = discreteSpace->dimensions().origin().getX() + discreteSpace->dimensions().extents().getX();
	float ymax = discreteSpace->dimensions().origin().getY() + discreteSpace->dimensions().extents().getY();
	
	float stepRatioCreationAgents = (2.0*procPerx/((xmax-xmin)*initialDensity));

	agentsToAdd = 0;
	if (stepRatioCreationAgents >= 1){
		if (repast::Random::instance()->nextDouble() < (1/stepRatioCreationAgents))
                	agentsToAdd = 1;
	}else{
		agentsToAdd = 1/stepRatioCreationAgents;
	}



	//Are we TOP?
	if ((agentsToAdd > 0) && (ymax == (HEIGHT + 1))){
		for (int i = 0; i < agentsToAdd; i++){
			bool _sick = false;
			if (repast::Random::instance()->nextDouble() < sickRate) _sick = true;
			bool _infected = _sick; //To be sick means to be infected too
			float _drifting = repast::Random::instance()->nextDouble() * drifting;
			float _stopping = repast::Random::instance()->nextDouble() * stopping;
			int _stopTime = repast::Random::instance()->nextDouble() * stopTime;
			float _speed = 0.2 * (2 * repast::Random::instance()->nextDouble() - 1) + speed;
			bool _hasApp = repast::Random::instance()->nextDouble() < applicationRate;
			bool _directionTop = false;

			//Get an empty position in the topLimit 

			repast::Point<int> initialLocation(0,0);

			j = 0;
			int randIndex = xmin - discreteSpace->bounds().origin().getX() + repast::Random::instance()->nextDouble() * (xmax - xmin) - 1;
			while (not checkPositionEmpty(topLimit[randIndex])){
				randIndex = xmin + repast::Random::instance()->nextDouble() * (xmax - xmin) - 1;
				if (j >= maxPositionsToLookForward) break;
				j++;
			}

			if (j > maxPositionsToLookForward) continue;

			initialLocation = topLimit[randIndex];

			repast::AgentId id(countOfAgents, rank, 0);
			id.currentRank(rank);

			RepastHPCAgent* agent = new RepastHPCAgent(this, id, _stopping, _stopTime, 0, repast::RepastProcess::instance()->getScheduleRunner().currentTick(), _drifting, _speed, _directionTop, 0, -1, 0, repast::RepastProcess::instance()->getScheduleRunner().currentTick(), infectiousness, encounterRadius, _sick, _infected, 0, phoneThreshold1, phoneThreshold2, _hasApp, signalRadius); //stopCounter=0, entryTime= tick, countInfected=0, infectionTime = -1, phoneActiveCount=0, timeSpentWithOthers = 0

			context.addAgent(agent);
			//TODO AMV: Quan s'afegeix un agent fora de la regió assignada, peta després en el balance
			discreteSpace->moveTo(id, initialLocation);
			countOfAgents++;
cout << "AMV(" << rank << ") xmax: " << (long)xmax << " bounds().extents().getX(): " << (long)discreteSpace->bounds().extents().getX() << endl;
cout << "AMV(" << rank << ") ymax: " << (long)ymax << " bounds().extents().getY(): " << (long)discreteSpace->bounds().extents().getY() << endl;

			std::cout << "Agent to add TOP: " << id << " " << initialLocation << " TICK: " << repast::RepastProcess::instance()->getScheduleRunner().currentTick() << std::endl;

   		}
	}

	//Are we BOTTON?
	if ((agentsToAdd > 0) && (round(ymin) == round(discreteSpace->bounds().origin().getY()))){
		for (int i = 0; i < agentsToAdd; i++){
			bool _sick = false;
			if (repast::Random::instance()->nextDouble() < sickRate) _sick = true;
			bool _infected = _sick; //To be sick means to be infected too
			float _drifting = repast::Random::instance()->nextDouble() * drifting;
			float _stopping = repast::Random::instance()->nextDouble() * stopping;
			int _stopTime = repast::Random::instance()->nextDouble() * stopTime;
			float _speed = 0.2 * (2 * repast::Random::instance()->nextDouble() - 1) + speed;
			bool _hasApp = repast::Random::instance()->nextDouble() < applicationRate;
			bool _directionTop = true;

			//Get an empty position in the botLimit 

			repast::Point<int> initialLocation(0,0);

			j = 0;
			int randIndex = xmin - discreteSpace->bounds().origin().getX() + repast::Random::instance()->nextDouble() * (xmax - xmin) - 1;
			while (not checkPositionEmpty(botLimit[randIndex])){
				randIndex = xmin + repast::Random::instance()->nextDouble() * (xmax - xmin) - 1;
				if (j >= maxPositionsToLookForward) break;
				j++;
			}

			if (j > maxPositionsToLookForward) continue;

			initialLocation = botLimit[randIndex];

			repast::AgentId id(countOfAgents, rank, 0);
			id.currentRank(rank);

			RepastHPCAgent* agent = new RepastHPCAgent(this, id, _stopping, _stopTime, 0, repast::RepastProcess::instance()->getScheduleRunner().currentTick(), _drifting, _speed, _directionTop, 0, -1, 0, repast::RepastProcess::instance()->getScheduleRunner().currentTick(), infectiousness, encounterRadius, _sick, _infected, 0, phoneThreshold1, phoneThreshold2, _hasApp, signalRadius); //stopCounter=0, entryTime= tick, countInfected=0, infectionTime = -1, phoneActiveCount=0, timeSpentWithOthers = 0

			context.addAgent(agent);
			//TODO AMV: Quan s'afegeix un agent fora de la regió assignada, peta després en el balance
			discreteSpace->moveTo(id, initialLocation);
			countOfAgents++;
cout << "AMV(" << rank << ") xmin: " << (long)xmin << " bounds().origin().getX(): " << (long)discreteSpace->bounds().origin().getX() << endl;
cout << "AMV(" << rank << ") ymin: " << (long)ymin << " bounds().origin().getY(): " << (long)discreteSpace->bounds().origin().getY() << endl;

			std::cout << "Agent to add BOTTON: " << id << " " << initialLocation << std::endl;
   		}
	}



	//Get new agent vector to take into account new and death agents
	agents.clear();
	context.selectAgents(repast::SharedContext<RepastHPCAgent>::LOCAL, agents);

	// TODO: segment Encounters BEGIN per comentar
	it = agents.begin();
	i = 0;
	while(it != agents.end()){
                (*it)->countEncountersReal(discreteSpace);
                (*it)->countEncountersRecorded(discreteSpace);
                it++;
	}
	// TODO JJ: segment ENCOunters END per comentar



	discreteSpace->balance(); 

	repast::RepastProcess::instance()->synchronizeAgentStatus<RepastHPCAgent, RepastHPCAgentPackage, RepastHPCAgentPackageProvider, RepastHPCAgentPackageReceiver>(context, *provider, *receiver, *receiver);
    
    repast::RepastProcess::instance()->synchronizeProjectionInfo<RepastHPCAgent, RepastHPCAgentPackage, RepastHPCAgentPackageProvider, RepastHPCAgentPackageReceiver>(context, *provider, *receiver, *receiver);

	repast::RepastProcess::instance()->synchronizeAgentStates<RepastHPCAgentPackage, RepastHPCAgentPackageProvider, RepastHPCAgentPackageReceiver>(*provider, *receiver);
    
}

/*
 *    Class: RepastHPCModel
 * Function: initSchedule
 * --------------------
 * Schedulle what to do in every simulation time
 * 
 * runner: scheduler runner identificacion
 *
 * returns: -
 */
void RepastHPCModel::initSchedule(repast::ScheduleRunner& runner){
	runner.scheduleEvent(2, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::doSomething)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::recordResults)));
	runner.scheduleStop(stopAt);
	
	// Data collection
	runner.scheduleEvent(1.5, 5, repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::record)));
	runner.scheduleEvent(10.6, 10, repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::write)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::record)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::write)));
	runner.scheduleEvent(10, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::printAgentsPosition)));
	runner.scheduleEvent(1800, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::printAgentsPosition)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::printAgentsPosition)));
}

/*
 *    Class: RepastHPCModel
 * Function: recordResults
 * --------------------
 * Record results
 * 
 * -: -
 *
 * returns: -
 */
void RepastHPCModel::recordResults(){
	if(repast::RepastProcess::instance()->rank() == 0){
		props->putProperty("Result","Passed");
		std::vector<std::string> keyOrder;
		keyOrder.push_back("RunNumber");
		keyOrder.push_back("stop.at");
		keyOrder.push_back("Result");
		props->writeToSVFile("./output/results.csv", keyOrder);
    }
}

/*
 *    Class: RepastHPCModel
 * Function: insideWorld
 * --------------------
 * Check if agentLoc is inside the world
 * 
 * agentLoc: Agent location
 *
 * returns: true if location is inside the world
 */
bool RepastHPCModel::insideWorld(std::vector<int> agentLoc){
	OGRPoint p1 = OGRPoint(agentLoc[0],agentLoc[1]);

	bool ret = p1.Within(&Polygon);

	return ret;
}

/*
 *    Class: RepastHPCModel
 * Function: cheackPositionEmpty
 * --------------------
 * Check if agentLoc is empty
 * 
 * agentLoc: Agent location
 *
 * returns: true if location is empty
 */

bool RepastHPCModel::checkPositionEmpty(repast::Point<int> agentLoc){ 
	//cout << "IsEmpty? agentLoc: " << agentLoc << endl;
	if (discreteSpace->getObjectAt(agentLoc) == NULL) return true;
	else return false;
}


/*
 *    Class: RepastHPCModel
 * Function: loadPolygon
 * --------------------
 * Load Polygon geometry
 * 
 *
 * returns: true it there is a GIS polygon
 */

bool RepastHPCModel::loadPolygon(repast::Point<double> *origin, repast::Point<double> *extent){ 

	GDALAllRegister();

	GDALDatasetUniquePtr poDS(GDALDataset::Open(initialGISFile.c_str(), GDAL_OF_VECTOR));
	if( poDS == nullptr ){
		printf( "Open failed.\n" );
		exit( 1 );
	}

	//Look for the first polygon in layers
	//TODO: look for a polygon with specific ID?. And this ID should be a parameter?
	for( const OGRLayer* poLayer: poDS->GetLayers()) {
		//printf( "Layer loop.\n" );
		for( const auto& poFeature: *poLayer ){
			//printf( "Feature loop.\n" );
			OGRGeometry *poGeometry;
			poGeometry = poFeature->GetGeometryRef();
			if( poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon ){
				poPolygon = poGeometry->toPolygon();
				Polygon = *poPolygon;//To save object

				repast::Point<double> end((*origin)[0]+(*extent)[0],(*origin)[1]+(*extent)[1]);

				//https://stackoverflow.com/questions/18747370/how-to-extract-vertexes-of-geometries-in-esri-shapefiles-using-ogr-library-with	
				OGRPoint ptTemp;
				OGRLinearRing *poExteriorRing = poPolygon ->getExteriorRing();
           			int NumberOfExteriorRingVertices = poExteriorRing ->getNumPoints();
				for ( int k = 0; k < NumberOfExteriorRingVertices; k++ ){
					poExteriorRing ->getPoint(k,&ptTemp);
					if (k == 0){
						(*origin)[0] = ptTemp.getX();
						(*origin)[1] = ptTemp.getY();
						end[0] = ptTemp.getX();
						end[1] = ptTemp.getY();
					}
					else{
						if (((*origin)[0]*(*origin)[0]+(*origin)[1]*(*origin)[1])>(ptTemp.getX()*ptTemp.getX()+ptTemp.getY()*ptTemp.getY())){ 
							(*origin)[0] = ptTemp.getX();
							(*origin)[1] = ptTemp.getY();
						}
						if ((end[0]*end[0]+end[1]*end[1])<(ptTemp.getX()*ptTemp.getX()+ptTemp.getY()*ptTemp.getY())){ 
							end[0] = ptTemp.getX();
							end[1] = ptTemp.getY();
						}

					}
					
				}
				(*extent)[0] = (int)(end[0]-(*origin)[0]) + 10;
				(*extent)[1] = (int)(end[1]-(*origin)[1]) + 10;

				(*origin)[0] = (*origin)[0] - 5; 
				(*origin)[1] = (*origin)[1] - 5; 
				
				return true;
				}
			else{
                                printf( "no Polygon geometry\n" );
				return false;
                        }
		}

	}
}


