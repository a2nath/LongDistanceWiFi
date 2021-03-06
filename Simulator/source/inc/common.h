#pragma once
#define _USE_MATH_DEFINES
#include <random>
#include <unordered_map>
#include <map>
#include <memory>
#include "Links.h"
#include "Logger.h"
#include <Windows.h>
#include <sstream>

#define DETERMINISTIC
//#define SHOWGUI
#define SHOWOUT
//#define REDUNDANT_RETRIES

#define error_out( s )            \
{                             \
	logs.done(" with error ");				   \
	std::wostringstream os_;    \
	os_ << s << "\n";                   \
	OutputDebugStringW( os_.str().c_str() );	\
	throw;	\
}

#define debugout( p )            \
{                             \
	std::wostringstream os_;    \
	os_ << p;                   \
	OutputDebugStringW( os_.str().c_str() );	\
}
typedef unsigned int uint;
typedef uint rts_release_time;
typedef uint rts_queued_time;
typedef uint frame_type;
typedef uint frame_sequence_num;
typedef int retry_count;
typedef uint station_number;
typedef uint mcs_index;
typedef uint data_rate;
typedef uint prop_del_us;
typedef std::string station_name;

typedef std::vector<uint> uivector;
typedef std::vector<double> doublevector;
typedef std::vector<std::vector<float>> floattable;

enum enm {
	conns = 0,
	/* Medium load in megabits per second */
	payload_rate = 1,
	/* TCP or UDP */
	contype = 2,
	progdur = 3,
	simdur = 4,
	/* Size of data payload in bytes */
	datbytes = 5,
	bwidth = 6,
	dsegments = 7,
	atout = 8,
	antpower = 9,
	stanames = 10,
	debugend = 11,
	pfactor = 12,
	chwdow = 13,
	seed = 14,
	acwmax = 15,
	acwmin = 16,
	relim = 17
};
/*
IDLE is for the GUI package.
Not used for the Simulation package.
*/
enum transciever_mode { IDLE = 0b00, TX = 0b10, RX = 0b01 };
const float lightspeed = 3e8;

template<typename T, typename U>
using umap = std::unordered_map<T, U>;
template <class U> float sum(std::vector<U> &list)
{
	float sum = 0;
	for (auto &s : list)
		sum += s;
	return sum;
}
template <class U> float ave(std::vector<U> &list)
{

	return sum(list) / (float)list.size();
}

/* Logging Objects */
struct Global {
	static umap<uint, std::string> mcs_vs_modname;
	static umap<std::string, double> data_bits_per_OFDM_symbol;
	static umap<std::string, float> PER_thrsh;
	static umap<std::string, umap<uint, double>> data_rates;
	static umap<std::string, std::map<float, double>> SER_TABLE;
	static umap<std::string, umap<uint, float>> receive_busy_threshold;
	static Link connections;
	static uint station_count;
	static uint data_pack_size;
	static uint data_fragments;
	static uint channel_number;
	static uint traffic_type;
	static std::vector<unsigned long int> seeds;
	// in Watts
	static std::vector<float> txpowers;
	static std::multimap<station_number, float> traffic_load;
	static std::string ap_station;
	static float bandwidth;
	static float frequency;
	static uint produration;
	static uint simduration;
	static bool adapt_int_tout;
	static std::map<std::string, uint> sta_name_map;
	static float prop_factor;
	static uint DEBUG_END;
	static uint chwindow;
	static uint aCWmax;
	static uint aCWmin;
	static uint dot11ShortRetryLimit;
	enum frame_map { _DATA = 0b0000, _RTS = 0b1011, _CTS = 0b1100, _ACK = 0b1101 };
};
struct Logs {
	vector<Logger*> stations;
	Logger *through;
	Logger *common;
	void done(string s = "")
	{
		auto name = common->getname();
		for (auto &sta : stations) { sta->writeline("======= Sim ended at " + s + "ms ======="); delete sta; }
		for (auto p : { through,common }) delete p;
		system(("notepad++.exe " + name).c_str());
	}
};
extern Logs logs;


/* Global helper functions */
template<class T>
std::string num2str(const T &input) { return std::to_string(input); }
template<class T>
std::string num2str(const umap<uint, T>& input) { string output = ""; for (auto m : input) output += std::to_string(m.second) + ", "; return output; }
template<class T>
std::string num2str(const vector<T>& input) { string output = ""; for (auto m : input) output += std::to_string(m) + ","; return output; }
template<class T> T str2num(string &s)
{
	T x = 0;
	std::stringstream parser(s);
	parser >> x;
	return x;
};
int get_sinr2idx(float sinr);
double get_per(mcs_index mcs, float sinr);
double get_per(string mod_scheme, float sinr);
double mcs2bps(uint mcs);
double mcs2thru(uint mcs);
uint data_size(uint mpdu_info, mcs_index mcs);
uivector linspace(double a, double b, int n);
double phyStr2Mbps(std::string name);
double dBm2W(double dBm);
double W2dBm(double W);
double dB2lin(double dB);
double lin2dB(double dB);
float micro2s(double s);
uint s2micro(double s);
/* Converts from duration in usecond to slot-time unit
Inputs: float duration in usecond, boolean if want to round it down
Output: returns the slot number */
uint dur2slots(uint duration, bool round_down = 0);
uint slots2dur(uint slots);
uint nextslottime_us(uint duration);
uivector nextslottime_us(const uivector &input);
void dout(std::string message, bool error = 0);

/* Physical layer parameters */
// in dB
const float G_ap = 12;
// in dB
const float G_cl = 2;
// in MHz
const uint frequency = 530;
// in dB
const float system_noise_figure = 5;
// SER table Min SNR
const float snr_min = -10;
// SER table Max SNR
const float snr_max = 40;

/* .11a constants  in microseconds */
const uint dot11a_sifs = 16;
const uint dot11a_difs = 34;
const uint dot11a_eifs = 43;
const uint dot11a_slot_time = 9;
const uint symbolrate = 250000;
const uint aPHY_RX_START_Delay = 25;
const uint aPreambleLength = 16;
const uint aPLCPHeaderLength = 4;
const uint aMPDUMaxLength = 4095;
const uint aCCATime = 3;
const uint CTSTimeoutInterval = dot11a_sifs + dot11a_slot_time + aPHY_RX_START_Delay;
const uint DATTimeoutInterval = dot11a_sifs + 38 * dot11a_slot_time;
const uint ACKTimeoutInterval = dot11a_sifs + dot11a_slot_time + aPHY_RX_START_Delay;
const uint rts_nav_timeout_interval = 2 * dot11a_sifs + aPHY_RX_START_Delay + 2 * dot11a_slot_time;
const uint max_mcs_count = 8;

/* in number of slots for backoff window limits */
/* backoff algo: choose random num between [0, CW], where aCWmin < CW < aCWmax */
// in dBm
const float thermal_per_bandwidth = -174;

// in dBm
const float dot11_CCA_threshold = -82;

const uint RTSCTSACK_DUR = 5;
const uint DATA_DUR = 5;

//RTS threshold, to tell a station if RTS/CTS is required given the length of a short data frame, because handshaking is costly
//can be never, always, or if the size is above a certain threshold
const uint dot11RTSThreshold = 3000;
//retry mechanism short

// frame related
const uint service_tail = 22; //bits
const uint preamble_signal = 20; //us