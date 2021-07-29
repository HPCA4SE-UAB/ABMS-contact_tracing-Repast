/* Agent.h */
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



#ifndef AGENT
#define AGENT

#include "Phone.h"

#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"

class RepastHPCModel;

/* Agents */
class RepastHPCAgent{
	
private:
	RepastHPCModel* _model;
	repast::AgentId   	id_;

	/*Walker*/
	float _stopping; 
	int _stopTime; 
	int _stopCounter; 
	int _entryTime; 
	float _drifting; 
	float _speed; 
	bool _directionTop;
	/*HumanBeeing*/
	Phone* _phone; 
	int _countInfected;
	int _infectionTime;
	int _phoneActiveCount;
	int _timeInSimulation;
	double _infectiousness;
	int _encounterRadius;
	std::vector<std::pair<repast::AgentId,int>> _encountersReal; 
	std::vector<std::pair<repast::AgentId,int>> _encountersRecorded;
	/*Agent*/
	bool _sick; 
	bool _infected; 
	int _timeSpentWithOthers;
	/*Phone*/
	int _threshold1; 
	int _threshold2; 
	bool _hasApplication; 
	int _signalRadius;

public:
	RepastHPCAgent(){}
	RepastHPCAgent(RepastHPCModel* model, repast::AgentId id, float stopping, int stopTime, int stopCounter, int entryTime, float drifting, float speed, bool directionTop, int countInfected, int infectionTime, int phoneActiveCount, int timeInSimulation, double infectiousness, int encounterRadius, bool sick, bool infected, int timeSpentWithOthers, int threshold1, int threshold2, bool hasApplication, int signalRadius);
	RepastHPCAgent(RepastHPCModel* model, repast::AgentId id, float stopping, int stopTime, int stopCounter, int entryTime, float drifting, float speed, bool directionTop, int countInfected, int infectionTime, int phoneActiveCount, int timeInSimulation, double infectiousness, int encounterRadius, bool sick, bool infected, int timeSpentWithOthers, int threshold1, int threshold2, bool hasApplication, int signalRadius, std::vector<std::pair<repast::AgentId,int>> encountersReal, std::vector<std::pair<repast::AgentId,int>> encountersRecorded, Phone* phone);
	
    ~RepastHPCAgent();
	
    /* Required Getters */
    virtual repast::AgentId& getId(){                   return id_;    }
    virtual const repast::AgentId& getId() const {      return id_;    }
	
    /* Getters specific to this kind of Agent */
	/*Walker*/
	float getStopping();
	int getStopTime();
	float getDrifting(); 
	float getSpeed();	
	bool directionTop(); 
	int getStopCounter();
	int getEntryTime(); 
	/*HumaniBeeing*/
	int getCountInfected(); 
	int getInfectionTime(); 
	int getPhoneActiveCount(); 
	int getTimeInSimulation(); 
	int getEncounterRadius();
	double getInfectiousness(); 
	bool isSick(); 
	bool isInfected(); 

	std::vector<std::pair<repast::AgentId,int>> getEncountersReal();
	std::vector<std::pair<repast::AgentId,int>> getEncountersRecorded();
	Phone* getPhone();


	/*Agent*/
	int getTimeSpentWithOthers();
    /*Phone*/
    int getThreshold1();
    int getThreshold2();
    bool getHasApplication();
    int getSignalRadius();

    /* Setters */
	void update(int currentRank, float stopping, int stopTime, int stopCounter, int entryTime, float drifting, float speed, bool directionTop, int countInfected, int infectionTime, int phoneActiveCount, int timeInSimulation, double infectiousness, int encounterRadius, bool sick, bool infected, int timeSpentWithOthers, int threshold1, int threshold2, bool hasApplication, int signalRadius, std::vector<std::pair<repast::AgentId,int>> encountersReal, std::vector<std::pair<repast::AgentId,int>> encountersRecorded, std::vector<int> buckets, std::vector<int> bucketsSick); 
	/*Walker*/
	void setDirectionTop();
	void setDirectionBot();
	void setStopCount(const int& count);
	/*HumanBeeing*/
	void setInfectionTime(const int& infectionTime);
	void incCountInfected();
	void printEncounters(); 
	void getInfected();  
	void countEncountersReal(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space);
	void countEncountersRecorded(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space);
	bool phoneBroadcast();
	/*Others functions*/	
    double frand();
    bool isIntoCircle(int x, int y, int xCircle, int yCircle, int rCircle);    

    /* Actions */
    void infect(repast::SharedContext<RepastHPCAgent>* context,
              repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space);
    bool move(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space);
    
};

/* Serializable Agent Package */
struct RepastHPCAgentPackage {
	
public:
	int    _id;
	int    _rank;
	int    _type;
	int    _currentRank;

	float _stopping;
	int _stopTime;
	int _stopCounter;
	int _entryTime;
	float _drifting;
	float _speed;
	bool _directionTop;

	int _countInfected;
	int _infectionTime;
	int _phoneActiveCount;
	int _timeInSimulation;
	double _infectiousness;
	int _encounterRadius;

	bool _sick;
	bool _infected;
	int _timeSpentWithOthers;

    int _threshold1;
    int _threshold2;
    bool _hasApplication;
    int _signalRadius;

    std::vector<std::pair<repast::AgentId,int>> _encountersReal; 
    std::vector<std::pair<repast::AgentId,int>> _encountersRecorded;

	std::vector<int> _buckets;
	std::vector<int> _bucketsSick;

	
    /* Constructors */
    RepastHPCAgentPackage(); // For serialization
    RepastHPCAgentPackage(int id, int rank, int type, int currentRank, float stopping, int stopTime, int stopCounter, int entryTime, float drifting, float speed, bool directionTop, int countInfected, int infectionTime, int phoneActiveCount, int timeInSimulation, double infectiousness, int encounterRadius, bool sick, bool infected, int timeSpentWithOthersi, int threshold1, int threshold2, bool hasApplication, int signalRadius, std::vector<std::pair<repast::AgentId,int>> encountersReal, std::vector<std::pair<repast::AgentId,int>> encountersRecorded, std::vector<int> buckets, std::vector<int> bucketsSick);
	
    /* For archive packaging */
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){
    ar & _id;
    ar & _rank;
    ar & _type;
    ar & _currentRank;
	ar & _stopping;
	ar & _stopTime;
	ar & _stopCounter;
	ar & _entryTime;
	ar & _drifting;
	ar & _speed;
	ar & _directionTop;
	ar & _countInfected;
	ar & _infectionTime;
	ar & _phoneActiveCount;
	ar & _timeInSimulation;
	ar & _infectiousness;
	ar & _encounterRadius;
	ar & _sick;
	ar & _infected;
	ar & _timeSpentWithOthers;
	ar & _threshold1;
	ar & _threshold2;
	ar & _hasApplication;
	ar & _signalRadius;
	ar & _encountersReal;
	ar & _encountersRecorded;
	ar & _buckets;
	ar & _bucketsSick;
    }
};


#endif
