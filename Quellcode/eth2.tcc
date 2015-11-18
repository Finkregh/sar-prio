dev "eth2"{
    egress {
        class ( <$interactive_out> )                        // Services we access
            if ip_tos_delay == 1 && tcp_dport == 22         // SSH + desire for short delay,
                                                            // outdated by RFC2474, but some programs might still use
                                                            // keep ip_tos* at the front of the extression,
                                                            // is early ip-header-field
            if ip_len < 256 && tcp_dport == 22              // short SSH-packets
            if tcp_dport == 53 || udp_dport == 53           // DNS
            if ip_len < 512 && tcp_dport == 80;             // short HTTP

        class ( <$interactive_in> )                         // Answers on existant connections (e.g. ACKs)
            if ip_tos_delay == 1 && tcp_sport == 22         // SSH + desire for short delay,
                                                            // outdated by RFC2474, but some programs might still use
            if ip_len < 256 && tcp_sport == 22              // short SSH-packets

        class ( <$other> )     if 1;                        // Always returns true. Leftover packets


        // Configure how much bandwidth the classes get
        htb () {                                            // Hierarchical Token Bucket packet scheduler
                                                            // <http://luxik.cdi.cz/~devik/qos/htb/>

            class ( rate 50Mbps, ceil 50Mbps ) {            // rate = how much is reserved, ceil = how much it can use max(*@\label{lst:eth2.tcc.eth2-speed}@*)
                // Allow $interactive to use up to 1Mbps, but make sure there's always 128kbps available
                $interactive_out    = class ( rate 128kbps, ceil 1Mbps, burst 128kb, cburst 1Mb ) { sfq; } ; // Stochastic Fairness Queuing,
                                                            // inhibits dominations of one connection inside the class
                $interactive_in     = class ( rate 128kbps, ceil 1Mbps, burst 123kb, cburst 1Mb ) { sfq; } ;
                $other     = class ( rate 1Mbps, ceil 49Mbps, burst 1Mb, cburst 49Mb ) { sfq; } ;
            }
        }
    }
}
