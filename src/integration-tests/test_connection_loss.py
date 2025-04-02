# Copyright 2024 Bloomberg Finance L.P.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
This suite of test cases exercises connection losses.
"""

from blazingmq.dev.it.fixtures import (  # pylint: disable=unused-import
    Cluster,
    order,
    cluster,
    tweak,
)
from blazingmq.dev.it.process.client import Client
from blazingmq.dev.it.process.proc import Process

pytestmark = order(1)


def test_broker_client(cluster: Cluster) -> None:
    """
    Test: connection loss between a broker and a client.
    - Start a broker and save the port it is listening.
    - Start a tproxy redirecting to the broker started on the previous step.
    - Start a client and connect it to the tproxy started on the previous step.
    - Kill the tproxy to brake the connection between the client and the broker.

    Concerns:
    - The client and the broker are able to detect the connection loss.
    - The connection is restored after tproxy restart.
    """

    proxies = cluster.proxy_cycle()
    proxy = next(proxies)
    broker_host = proxy.config.host
    broker_port = int(proxy.config.port)

    # Start tproxy between broker's and client's ports
    tproxy = Process("tproxy", ["tproxy", "-r", f"{broker_host}:{broker_port}"])
    tproxy.start()
    tproxy_port = tproxy.capture(r"Listening on (.+):(\d+)", timeout=2).group(2)

    # Start a client
    client: Client = proxy.create_client(f"client@{proxy.name}", port=tproxy_port)
    client.start_session()
    assert client.capture(r"CONNECTED", 5)

    # Kill tproxy to brake the connection between broker and client
    tproxy.kill()
    assert client.wait_connection_lost(5)

    # Start tproxy to restore the connection between broker and client
    tproxy = Process(
        "tproxy",
        ["tproxy", "-r", f"{broker_host}:{broker_port}", "-p", f"{tproxy_port}"],
    )
    tproxy.start()
    assert client.capture(r"RECONNECTED", 5)

    client.exit_gracefully()
    client.wait(5)
    tproxy.kill()
