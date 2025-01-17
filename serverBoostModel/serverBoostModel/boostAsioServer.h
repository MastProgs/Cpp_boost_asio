// 디렉토리 파일 위치				..\boost_1_62_0\boost_1_62_0;				%(AdditionalIncludeDirectories)
// 링커 추가 라이브러리 디렉토리	..\boost_1_62_0\boost_1_62_0\stage\lib;		%(AdditionalLibraryDirectories)

#pragma once
#include<boost\asio.hpp>

static boost::asio::io_service g_io_service;

using boost::asio::ip::tcp;

class player_session : public std::enable_shared_from_this<player_session>
{
public:
	player_session(tcp::socket s, const unsigned int& index) : m_socket(std::move(s)), m_id(index) {};
	~player_session() {};

	void Init();

	bool get_current_connect_state() { return m_connect_state; }
private:
	// Function
	void m_recv_packet();
	void m_process_packet(const unsigned int& id, Packet buf[]);
	void send_packet(const unsigned int& id, Packet *packet);

	// 통신용 변수
	tcp::socket		m_socket;

	// 플레이어 상태 변수
	bool m_connect_state{ false };
	unsigned int m_id{ 0 };

	// 버퍼 변수
	Packet m_recv_buf[MAX_BUF_SIZE]{ 0 };
	Packet m_data_buf[MAX_BUF_SIZE]{ 0 };
	unsigned int m_packet_size_current{ 0 };
	unsigned int m_packet_size_previous{ 0 };
};

class boostAsioServer
{
public:
	boostAsioServer();
	~boostAsioServer();
	
private:
	// Function
	void getMyServerIP();
	void CheckThisCPUcoreCount();

	void start_io_service();
	void acceptThread();

	// 통신용 변수
	tcp::acceptor	m_acceptor;
	tcp::socket		m_socket;

	// 플레이어 고유 id 변수
	unsigned int	m_playerIndex{ UINT_MAX };

	// 서버 기본 정보 관련 변수
	bool			m_ServerShutdown{ false };
	int				m_cpuCore{ 0 };
	vector<thread*> m_worker_threads;
};

// 플레이어가 담긴 변수
static vector<shared_ptr<player_session>> g_clients;