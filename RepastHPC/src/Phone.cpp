#include "Phone.h"


//TODO: dependency PANDORA code
//#include <GeneralState.hxx>

#include <math.h>
#include "repast_hpc/Random.h"

Phone::Phone(const int& threshold1, const int& threshold2, const bool& hasApplicattion, const int& signalRadius) 
: _threshold1(threshold1), _threshold2(threshold2), _hasApplication(hasApplicattion), _signalRadius(signalRadius), _variance(5),
    _maxDistance(100) {
	//TODO: Obtain Uniform Value
    // _startOffset = Engine::GeneralState::statistics().getUniformDistValue(0,300);
	_startOffset = (int) repast::Random::instance()->nextDouble()*300+0.5;
    _buckets = std::vector<int>(3);
    _bucketsSick = std::vector<int>(3);
}

Phone::~Phone() {}

bool Phone::hasApp() {
    return _hasApplication;
}

std::vector<int> Phone::getBuckets() {
    return _buckets;
}

void Phone::setBuckets(std::vector<int> buckets) {
    _buckets = buckets;
}

std::vector<int> Phone::getBucketsPositive() {
    return _bucketsSick;
}

void Phone::setBucketsPositive(std::vector<int> bucketsSick) {
    _bucketsSick = bucketsSick;
}

int Phone::getStartOffset() {
    return _startOffset;
}

int Phone::getSignalRadius() {
    return _signalRadius;
}

int Phone::checkDetection(const int& sick, const double& distance, const int& dt) {
    int d_ref = 9;
	int s_ref = 70;
	int s_base = 40;
    double s_center = s_base + std::log(distance)*(s_ref-s_base)/std::log(d_ref);
    double noise = 2 * _variance * std::min((s_center-s_base)/20,1.0);
	//TODO: Here is using a std random generators (from cpp11).
    // std::default_random_engine generator;
    // std::normal_distribution<double> normal(s_center,noise);
    // double signal = normal(generator);
	double signal = repast::Random::instance()->createNormalGenerator(s_center,noise).next();


    int bucket = -1;
    int detect = 0;
    if (signal < _threshold1) {
        bucket = 0;
        detect = 1;
    }
    else if (signal >= _threshold1 and signal < _threshold2) {
        bucket = 1;
        detect = 1;
    }
    else if (signal >= _threshold2 and signal < _maxDistance) {
        bucket = 2;
    }
    if (bucket > -1) {
        _buckets[bucket] = _buckets[bucket] + dt;
        if (sick) {
            _bucketsSick[bucket] = _bucketsSick[bucket] + dt;
        }
    }
    return detect;
}

int Phone::getFirstBucketNotifications() {
    return _buckets[0]/900;
}

int Phone::getCombinedNotifications() {
    return (_buckets[0] + 0.5*_buckets[1])/900;
}

int Phone::getFirstBucketSickNotifications() {
    return _bucketsSick[0]/900;
}

int Phone::getCombinedSickNotifications() {
    return (_bucketsSick[0] + 0.5*_bucketsSick[1])/900;
}

