#ifndef FURYCANARYSHUTDOWNTASK_H_
#define FURYCANARYSHUTDOWNTASK_H_

#include "server/ServerCore.h"

class FuryCanaryShutdownTask : public Task {
	uint64 listingId = 0;
	int attempts = 0;

	void retry(const String& reason) {
		attempts++;

		if (attempts >= 30) {
			Logger::console.error()
				<< "FURY market canary: unable to queue graceful shutdown after "
				<< attempts << " attempts, listing=" << listingId
				<< ", reason=" << reason;
			return;
		}

		Logger::console.warning()
			<< "FURY market canary: shutdown command pipe not ready; retrying, listing="
			<< listingId << ", attempt=" << attempts
			<< ", reason=" << reason;

		schedule(1000);
	}

public:
	explicit FuryCanaryShutdownTask(uint64 oid) : listingId(oid) {
	}

	void run() override {
		ServerCore* core = ServerCore::getInstance();

		if (core == nullptr) {
			retry("ServerCore unavailable");
			return;
		}

		try {
			core->queueConsoleCommand("shutdown 0");

			Logger::console.info(true)
				<< "FURY market canary success; queued graceful shutdown, listing="
				<< listingId;
		} catch (const Exception& e) {
			retry(e.getMessage());
		} catch (...) {
			retry("unknown exception");
		}
	}
};

#endif // FURYCANARYSHUTDOWNTASK_H_
