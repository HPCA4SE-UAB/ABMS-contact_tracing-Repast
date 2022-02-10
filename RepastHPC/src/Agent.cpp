/* Agent.cpp */
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

#include "Agent.h"
#include "repast_hpc/Moore2DGridQuery.h"
#include "repast_hpc/Point.h"
#include <math.h>
#include <cmath>
#include <string.h>
#include "Model.h"

/*
 *    Class: RepastHPCAgent  
 * Function: RepastHPCAgent
 * --------------------
 * RepastHPCAgent class constructor
 * 
 *
 * returns: -
 */
RepastHPCAgent::RepastHPCAgent(RepastHPCModel* model, repast::AgentId id, float stopping, int stopTime, int stopCounter, int entryTime, float drifting, float speed, bool directionTop, int countInfected, int infectionTime, int phoneActiveCount, int timeInSimulation, double infectiousness, int encounterRadius, bool sick, bool infected, int timeSpentWithOthers, int threshold1, int threshold2, bool hasApplication, int signalRadius):_model(model), id_(id), _stopping(stopping), _stopTime(stopTime), _stopCounter(stopCounter), _entryTime(entryTime), _drifting(drifting), _speed(speed), _directionTop(directionTop), _countInfected(countInfected), _infectionTime(infectionTime), _phoneActiveCount(phoneActiveCount), _timeInSimulation(timeInSimulation), _infectiousness(infectiousness), _encounterRadius(encounterRadius), _sick(sick), _infected(infected), _timeSpentWithOthers(timeSpentWithOthers), _threshold1(threshold1), _threshold2(threshold2), _hasApplication(hasApplication), _signalRadius(signalRadius){
	_phone = new Phone(threshold1,threshold2,hasApplication,signalRadius);
}

RepastHPCAgent::RepastHPCAgent(RepastHPCModel* model, repast::AgentId id, float stopping, int stopTime, int stopCounter, int entryTime, float drifting, float speed, bool directionTop, int countInfected, int infectionTime, int phoneActiveCount, int timeInSimulation, double infectiousness, int encounterRadius, bool sick, bool infected, int timeSpentWithOthers, int threshold1, int threshold2, bool hasApplication, int signalRadius, std::vector<std::pair<repast::AgentId,int>> encountersReal, std::vector<std::pair<repast::AgentId,int>> encountersRecorded, Phone* phone):_model(model), id_(id), _stopping(stopping), _stopTime(stopTime), _stopCounter(stopCounter), _entryTime(entryTime), _drifting(drifting), _speed(speed), _directionTop(directionTop), _countInfected(countInfected), _infectionTime(infectionTime), _phoneActiveCount(phoneActiveCount), _timeInSimulation(timeInSimulation), _infectiousness(infectiousness), _encounterRadius(encounterRadius), _sick(sick), _infected(infected), _timeSpentWithOthers(timeSpentWithOthers), _threshold1(threshold1), _threshold2(threshold2), _hasApplication(hasApplication), _signalRadius(signalRadius), _encountersReal(encountersReal),  _encountersRecorded(encountersRecorded) {
	_phone = phone;
    _phone->setBuckets(phone->getBuckets());
    _phone->setBucketsPositive(phone->getBucketsPositive());
}


/*
 *    Class: RepastHPCAgent  
 * Function: ~RepastHPCAgent
 * --------------------
 * RepastHPCAgent class destructor
 * 
 * -: -
 *
 * returns: -
 */
RepastHPCAgent::~RepastHPCAgent(){ 
}

/*
 *    Class: RepastHPCAgent  
 * Function: getStopping
 * --------------------
 * Get _stopping
 * 
 * -: -
 *
 * returns: _stopping
 */
float RepastHPCAgent::getStopping() {
    return _stopping;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getStopTime
 * --------------------
 * Get _stopping
 * 
 * -: -
 *
 * returns: _stopTime
 */
int RepastHPCAgent::getStopTime() {
    return _stopTime;
}



/*
 *    Class: RepastHPCAgent  
 * Function: getDrifting
 * --------------------
 * Get _drifting
 * 
 * -: -
 *
 * returns: _drifting
 */
float RepastHPCAgent::getDrifting() {
    return _drifting;
}



/*
 *    Class: RepastHPCAgent  
 * Function: getSpeed
 * --------------------
 * Get _speed
 * 
 * -: -
 *
 * returns: _speed
 */
float RepastHPCAgent::getSpeed() {
    return _speed;
}

/*
 *    Class: RepastHPCAgent  
 * Function: directionTop
 * --------------------
 * Get _directionTop
 * 
 * -: -
 *
 * returns: _directionTop
 */
bool RepastHPCAgent::directionTop() {
    return _directionTop;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getEncounterRadius
 * --------------------
 * Get _encounterRadius
 * 
 * -: -
 *
 * returns: _encounterRadius
 */
int RepastHPCAgent::getEncounterRadius() {
    return _encounterRadius;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getInfected
 * --------------------
 * Set _infected
 * 
 * -: -
 *
 * returns: _infected
 */
void RepastHPCAgent::getInfected() {
    _infected = true;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getInfectiousness
 * --------------------
 * Get _infectiousness
 * 
 * -: -
 *
 * returns: _infectiousness
 */
double RepastHPCAgent::getInfectiousness() {
    return _infectiousness;
}

/*
 *    Class: RepastHPCAgent  
 * Function: isSick
 * --------------------
 * Get _sick
 * 
 * -: -
 *
 * returns: _sick
 */
bool RepastHPCAgent::isSick() {
    return _sick;
}


/*
 *    Class: RepastHPCAgent  
 * Function: isInfected
 * --------------------
 * Get _infected
 * 
 * -: -
 *
 * returns: _infected
 */
bool RepastHPCAgent::isInfected() {
    return _infected;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getStopCounter
 * --------------------
 * Get _stopCounter
 * 
 * -: -
 *
 * returns: _stopCounter
 */
int RepastHPCAgent::getStopCounter(){
    return _stopCounter;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getEntryTime
 * --------------------
 * Get _entryTime
 * 
 * -: -
 *
 * returns: _entryTime
 */
int RepastHPCAgent::getEntryTime(){
    return _entryTime;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getCountInfected
 * --------------------
 * Get _countInfected
 * 
 * -: -
 *
 * returns: _countInfected
 */   
int RepastHPCAgent::getCountInfected(){
    return _countInfected;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getInfectionTime
 * --------------------
 * Get _infectionTime
 * 
 * -: -
 *
 * returns: _infectionTime
 */
int RepastHPCAgent::getInfectionTime(){
    return _infectionTime;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getPhoneActiveCount
 * --------------------
 * Get _phoneActiveCount
 * 
 * -: -
 *
 * returns: _phoneActiveCount
 */   
int RepastHPCAgent::getPhoneActiveCount(){
    return _phoneActiveCount;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getTimeInSimulation
 * --------------------
 * Get _timeInSimulation
 * 
 * -: -
 *
 * returns: _timeInSimulation
 */
   
int RepastHPCAgent::getTimeInSimulation() {
    return _timeInSimulation;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getTimeSpentWithOthers
 * --------------------
 * Get _timeSpentWithOthers
 * 
 * -: -
 *
 * returns: _timeSpentWithOthers
 */   
int RepastHPCAgent::getTimeSpentWithOthers(){
    return _timeSpentWithOthers;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getThreshold1
 * --------------------
 * Get _threshold1
 * 
 * -: -
 *
 * returns: _threshold1
 */
int RepastHPCAgent::getThreshold1(){
	return _threshold1;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getThreshold2
 * --------------------
 * Get _threshold2
 * 
 * -: -
 *
 * returns: _threshold2
 */
int RepastHPCAgent::getThreshold2(){
	return _threshold2;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getHasApplication
 * --------------------
 * Get _hasApplication
 * 
 * -: -
 *
 * returns: _hasApplicattion
 */ 
bool RepastHPCAgent::getHasApplication(){
	return _hasApplication;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getSignalRadius
 * --------------------
 * Get _signalRadius
 * 
 * -: -
 *
 * returns: _signalRadius
 */
int RepastHPCAgent::getSignalRadius(){
	return _signalRadius;
}


/*
 *    Class: RepastHPCAgent  
 * Function: getEncountersReal
 * --------------------
 * Get _encountersReal
 * 
 * -: -
 *
 * returns: _encountersReal
 */
std::vector<std::pair<repast::AgentId,int>> RepastHPCAgent::getEncountersReal(){
	return _encountersReal;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getEncountersRecorded
 * --------------------
 * Get _encountersRecorded
 * 
 * -: -
 *
 * returns: _encountersRecorded
 */
std::vector<std::pair<repast::AgentId,int>> RepastHPCAgent::getEncountersRecorded(){
	return _encountersRecorded;
}

/*
 *    Class: RepastHPCAgent  
 * Function: getPhone
 * --------------------
 * Get _phone
 * 
 * -: -
 *
 * returns: _phone
 */
Phone* RepastHPCAgent::getPhone(){
	return _phone;
}

/*
 *    Class: RepastHPCAgent  
 * Function: update
 * --------------------
 * Update agent variables
 * 
 * -: -
 *
 * returns:
 */
void RepastHPCAgent::update(int currentRank, float stopping, int stopTime, int stopCounter, int entryTime, float drifting, float speed, bool directionTop, int countInfected, int infectionTime, int phoneActiveCount, int timeInSimulation, double infectiousness, int encounterRadius, bool sick, bool infected, int timeSpentWithOthers, int threshold1, int threshold2, bool hasApplication, int signalRadius, std::vector<std::pair<repast::AgentId,int>> encountersReal, std::vector<std::pair<repast::AgentId,int>> encountersRecorded, std::vector<int> buckets, std::vector<int> bucketsSick){
	id_.currentRank(currentRank);
	_stopping = stopping; 
	_stopTime = stopTime; 
	_stopCounter = stopCounter; 
	_entryTime = entryTime; 
	_drifting = drifting; 
	_speed = speed; 
	_directionTop = directionTop; 
	_countInfected = countInfected; 
	_infectionTime = infectionTime; 
	_phoneActiveCount = phoneActiveCount; 
	_timeInSimulation = timeInSimulation; 
	_infectiousness = infectiousness; 
	_encounterRadius = encounterRadius; 
	_sick = sick; 
	_infected = infected; 
	_timeSpentWithOthers = timeSpentWithOthers; 
	_threshold1 = threshold1; 
	_threshold2 = threshold2; 
	_hasApplication = hasApplication; 
	_signalRadius = signalRadius; 
	_encountersReal = encountersReal; 
	_encountersRecorded = encountersRecorded;
	_phone->setBuckets(buckets);
	_phone->setBucketsPositive(bucketsSick);
}

/*
 *    Class: RepastHPCAgent  
 * Function: setDirectionTop
 * --------------------
 * Set direction top
 * 
 * : -
 *
 * returns: 
 */
void RepastHPCAgent::setDirectionTop() {
    _directionTop = true;
}

/*
 *    Class: RepastHPCAgent  
 * Function: setDirectionBot
 * --------------------
 * Set direction botton
 * 
 * : -
 *
 * returns: 
 */
void RepastHPCAgent::setDirectionBot() {
    _directionTop = false;
}

/*
 *    Class: RepastHPCAgent  
 * Function: setStopCount
 * --------------------
 * Set _stopCounter
 * 
 * : -
 *
 * returns: 
 */
void RepastHPCAgent::setStopCount(const int& count) {
    _stopCounter = count;
}


/*
 *    Class: RepastHPCAgent  
 * Function: setInfectionTime
 * --------------------
 * Set _infectionTime
 * 
 * infectionTime: -
 *
 * returns: 
 */
void RepastHPCAgent::setInfectionTime(const int& infectionTime) {
    _infectionTime = infectionTime;
}


/*
 *    Class: RepastHPCAgent  
 * Function: incCountInfected(
 * --------------------
 * Inclement _countInfected
 * 
 * -: -
 *
 * returns: 
 */
void RepastHPCAgent::incCountInfected() {
    _countInfected++;
}

/*
 *    Class: RepastHPCAgent  
 * Function: printEncounters
 * --------------------
 * Print encounters
 * 
 * -: -
 *
 * returns: 
 */
void RepastHPCAgent::printEncounters() {
    std::cout << "Encounters real by: " <<  getId() << " #"<< _encountersReal.size() << std::endl;
    for (unsigned int i = 0; i < _encountersReal.size(); i++) std::cout << _encountersReal[i].first << "," << _encountersReal[i].second << " ";
    std::cout << std::endl;
    std::cout << "Encounters recorded by: " <<  getId() << " #"<< _encountersRecorded.size() << std::endl;
    for (unsigned int i = 0; i < _encountersRecorded.size(); i++) std::cout << _encountersRecorded[i].first << "," << _encountersRecorded[i].second << " ";
    std::cout << std::endl;
}

/*
 *    Class: RepastHPCAgent  
 * Function: countEncountersReal
 * --------------------
 * Register encounters
 * 
 * -: -
 *
 * returns: 
 */
void RepastHPCAgent::countEncountersReal(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space) {

	std::vector<RepastHPCAgent*> agentsEncounter;
	std::vector<int> agentLoc;
	std::vector<int> agentLocEncounter;

	space->getLocation(id_, agentLoc);
    repast::Point<int> center(agentLoc);
//cout << "id: "<< id_ << " agentLoc: " << agentLoc[0] << "," << agentLoc[1] << " center: " << center << " EncounterRadius: " << _encounterRadius << endl;
//cout << "id: "<< id_ << " center: " << center << " EncounterRadius: " << _encounterRadius << endl;
    repast::Moore2DGridQuery<RepastHPCAgent> moore2DQuery(space);
    moore2DQuery.query(center,  _encounterRadius, true, agentsEncounter);
    std::vector<RepastHPCAgent*>::iterator agentEncounter = agentsEncounter.begin();

	while(agentEncounter != agentsEncounter.end()){
		if ( id_ == ((*agentEncounter)->getId()) ){
    		agentEncounter++;
            continue; // Do not encounter himself
        }

		space->getLocation(((*agentEncounter)->getId()), agentLocEncounter);

        if (isIntoCircle(agentLoc[0], agentLoc[1], agentLocEncounter[0], agentLocEncounter[1], _encounterRadius)){
			if (_encountersReal.size() == 0) _encountersReal.push_back(std::pair<repast::AgentId,int>((*agentEncounter)->getId(),1));//TODO tipus retorn getId no és un string!!!! Funcionarà?
			else {
				bool in = false;
				for (unsigned int j = 0; j < _encountersReal.size(); j++) {
					if (_encountersReal[j].first == (*agentEncounter)->getId()) {
						in = true;
						_encountersReal[j].second += 1;
					}
				}
				if (not in) _encountersReal.push_back(std::pair<repast::AgentId,int>((*agentEncounter)->getId(),1));
 			}
			_timeSpentWithOthers += 1;
		}
		agentEncounter++;
	}
}


/*
 *    Class: RepastHPCAgent  
 * Function: countEncountersRecorded
 * --------------------
 * Register encounters
 * 
 * -: -
 *
 * returns: 
 */

void RepastHPCAgent::countEncountersRecorded(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space) {
	if (phoneBroadcast()) { 
        	std::vector<RepastHPCAgent*> agentsEncounter;
        	std::vector<int> agentLoc;
        	std::vector<int> agentLocEncounter;

        	space->getLocation(id_, agentLoc);
        	repast::Point<int> center(agentLoc);
        	repast::Moore2DGridQuery<RepastHPCAgent> moore2DQuery(space);
        	moore2DQuery.query(center, _phone->getSignalRadius(), true, agentsEncounter);
        	std::vector<RepastHPCAgent*>::iterator agentEncounter = agentsEncounter.begin();

        	while(agentEncounter != agentsEncounter.end()){
                	if ( id_ == ((*agentEncounter)->getId()) ){
                        	agentEncounter++;
                        	continue; // Do not encounter himself
                	}

                	space->getLocation(((*agentEncounter)->getId()), agentLocEncounter);

                	if (isIntoCircle(agentLoc[0], agentLoc[1], agentLocEncounter[0], agentLocEncounter[1], _encounterRadius)){
                        	if (_encountersRecorded.size() == 0) _encountersRecorded.push_back(std::pair<repast::AgentId,int>((*agentEncounter)->getId(),1));//TODO tipus retorn getId no és un string!!!! Funcionarà?
                        	else {
                                	bool in = false;
                                	for (unsigned int j = 0; j < _encountersRecorded.size(); j++) {
                                        	if (_encountersRecorded[j].first == (*agentEncounter)->getId()) {
                                                	in = true;
                                        	_encountersRecorded[j].second += 1;
                                        	}
                                	}
                        	if (not in)_encountersRecorded.push_back(std::pair<repast::AgentId,int>((*agentEncounter)->getId(),1));
                        	}
                	_timeSpentWithOthers += 1;
                	}
        	agentEncounter++;
        	}
	}
}


/*
 *    Class: RepastHPCAgent  
 * Function: phoneBroadcast
 * --------------------
 * Phone broadcast?
 * 
 * -: -
 *
 * returns: 
 */
bool RepastHPCAgent::phoneBroadcast() {
    if (_phoneActiveCount > 0) {
        _phoneActiveCount--;
        return true;
    }
    bool result = _phone->hasApp() and ((int)repast::RepastProcess::instance()->getScheduleRunner().currentTick() + _phone->getStartOffset())%300 == 0;
    if (result) _phoneActiveCount = 4;
    return result;
}


/*
 *    Class: RepastHPCAgent  
 * Function: frand
 * --------------------
 * Get a random number between 0 and RAND_MAX
 * 
 * -: -
 *
 * returns: random number
 */
double RepastHPCAgent::frand(void) {
	double value;
	value = ((double)rand()/(RAND_MAX));
	return value;
}

/*
 *    Class: RepastHPCAgent  
 * Function: isIntoCircle
 * --------------------
 * check if distance between two points is less than or equal
 * 
 * x,y: position A coordinates
 * xCircle,yCircle: posicion B coordinates
 * rCircle: distance to check between position A to B
 *
 * returns: true distance between A to B is less than or equal rCircle
 *          false distance between A to B is more than rCicle
 */
bool RepastHPCAgent::isIntoCircle(int x, int y, int xCircle, int yCircle, int rCircle) {
        double dist = sqrt(pow(x-xCircle, 2)+pow(y-yCircle, 2));

        if(dist <= rCircle) {
                return true;
        } else {
                return false;
        }
}



/*
 *    Class: RepastHPCAgent  
 * Function: infect 
 * --------------------
 * tray to infect all agents located until _encounterRadius distance
 *
 * context-: Repast context
 * space: Repast space
 *
 * returns: 
 */
void RepastHPCAgent::infect(repast::SharedContext<RepastHPCAgent>* context,
                              repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space){

	if (!isSick()) return;

	std::vector<RepastHPCAgent*> agentsToInfect;
    
	std::vector<int> agentLoc;
	std::vector<int> agentLocToInfect;

	space->getLocation(id_, agentLoc);
	repast::Point<int> center(agentLoc);
	repast::Moore2DGridQuery<RepastHPCAgent> moore2DQuery(space);
	moore2DQuery.query(center,  _encounterRadius, true, agentsToInfect);
    
	std::vector<RepastHPCAgent*>::iterator agentToInfect = agentsToInfect.begin();

	while(agentToInfect != agentsToInfect.end()){
		if ( id_ == ((*agentToInfect)->getId()) ){ 
			agentToInfect++;	
			continue; // Do not infect himself
		}

       	space->getLocation(((*agentToInfect)->getId()), agentLocToInfect);

		if (isIntoCircle(agentLoc[0], agentLoc[1], agentLocToInfect[0], agentLocToInfect[1], _encounterRadius)){

			double draw = repast::Random::instance()->nextDouble();

			if (not ((*agentToInfect)->isSick() or (*agentToInfect)->isInfected()) and draw < getInfectiousness()) {
                std::cout << "F " << getId() << " infected " << (*agentToInfect)->getId() << std::endl;
                (*agentToInfect)->getInfected();
                (*agentToInfect)->setInfectionTime(repast::RepastProcess::instance()->getScheduleRunner().currentTick());
                incCountInfected();
            }
		}
		agentToInfect++;
    	}
}

/*
 *    Class: RepastHPCAgent  
 * Function: move 
 * --------------------
 * move agent
 *
 * space: Repast space
 *
 * returns: true: die 
 */
bool RepastHPCAgent::move(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space){
	std::vector<int> agentLoc;
	std::vector<int> agentNewLoc;
	int maxPositionsToLookForward = 3, i = 0;//TODO maxPositionsToLookForward com a paràmetre?

    
	if (getStopCounter() > 0){ //I amb stoped
		setStopCount(getStopCounter() - 1);
		return false;
	}

	if (repast::Random::instance()->nextDouble() < getStopping()) {
		setStopCount((int) getStopTime() * repast::Random::instance()->nextDouble());
		return false;
	}
	else {
	space->getLocation(id_, agentLoc);
	int modX = 0;
	int modY = 0;
	if (directionTop()) modY = 1;
    	else modY = -1;
		do{
       		if (repast::Random::instance()->nextDouble() < getDrifting()) {
           		if (repast::Random::instance()->nextDouble() < 0.5) modX = (int)getSpeed() * 1;
           		else modX = (int)getSpeed() * -1;
       		}	

    		std::vector<int> agentNewLoct;
    		agentNewLoct.push_back(agentLoc[0] + modX);
    		agentNewLoct.push_back(agentLoc[1] + modY);
			agentNewLoc = agentNewLoct;

			i++;
			if (i >= maxPositionsToLookForward) return false; //Return without move

 		}while(!_model->checkPositionEmpty(agentNewLoc));

        if (_model->insideWorld(agentNewLoc)){
			space->moveTo(id_,agentNewLoc);
			return false;
		}
        else {
            printEncounters(); 
            return true;
        }
    }
}


/* Serializable Agent Package Data */

/*
 *
 *    Class: RepastHPCAgentPackage 
 * Function: RepastHPCAgentPackage
 * --------------------
 * RepastHPCAgentPackage constructor
 *
 * -: -
 *
 * returns: -
 */
RepastHPCAgentPackage::RepastHPCAgentPackage(){ 
//cout << "RepastHPCAgentPackage::RepastHPCAgentPackage" << endl;
}

/*
 *    Class: RepastHPCAgentPackage 
 * Function: RepastHPCAgentPackage
 * --------------------
 * RepastHPCAgentPackage constructor
 *
 *
 * returns: -
 */
RepastHPCAgentPackage::RepastHPCAgentPackage(int id, int rank, int type, int currentRank, float stopping, int stopTime, int stopCounter, int entryTime, float drifting, float speed, bool directionTop, int countInfected, int infectionTime, int phoneActiveCount, int timeInSimulation, double infectiousness, int encounterRadius, bool sick, bool infected, int timeSpentWithOthers, int threshold1, int threshold2, bool hasApplication, int signalRadius, std::vector<std::pair<repast::AgentId,int>> encountersReal, std::vector<std::pair<repast::AgentId,int>> encountersRecorded, std::vector<int> buckets, std::vector<int> bucketsSick):
_id(id), _rank(rank), _type(type), _currentRank(currentRank), _stopping(stopping), _stopTime(stopTime), _stopCounter(stopCounter), _entryTime(entryTime), _drifting(drifting), _speed(speed), _directionTop(directionTop), _countInfected(countInfected), _infectionTime(infectionTime), _phoneActiveCount(phoneActiveCount), _timeInSimulation(timeInSimulation), _infectiousness(infectiousness), _encounterRadius(encounterRadius), _sick(sick), _infected(infected), _timeSpentWithOthers(timeSpentWithOthers), _threshold1(threshold1), _threshold2(threshold2), _hasApplication(hasApplication), _signalRadius(signalRadius), _encountersReal(encountersReal),  _encountersRecorded(encountersRecorded), _buckets(buckets), _bucketsSick(bucketsSick) {

//cout << "RepastHPCAgentPackage::RepastHPCAgentPackage..." << endl;

}
