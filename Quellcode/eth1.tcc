dev "eth1"{							(*@\label{lst:eth1.tcc.eth1}@*)
    egress {							(*@\label{lst:eth1.tcc.egress}@*)
        class ( <$interactive_out> )                        // Services we access (*@\label{lst:eth1.tcc.interactive-out}@*)
            if ip_tos_delay == 1 && tcp_dport == 22         // SSH + desire for short delay,
                                                            // outdated by RFC2474, but some programs might still use
                                                            // keep ip_tos* at the front of the extression,
                                                            // is early ip-header-field
            if ip_len < 256 && tcp_dport == 22              // short SSH-packets
            if tcp_dport == 53 || udp_dport == 53           // DNS
            if ip_len < 512 && tcp_dport == 80;             // short HTTP

        class ( <$interactive_in> )                         // Answers on existant connections (e.g. ACKs) (*@\label{lst:eth1.tcc.interactive-in}@*)
            if ip_tos_delay == 1 && tcp_sport == 22         // SSH + desire for short delay,
                                                            // outdated by RFC2474, but some programs might still use
            if ip_len < 256 && tcp_sport == 22              // short SSH-packets

        class ( <$other> )     if 1;                        // Always returns true. Leftover packets (*@\label{lst:eth1.tcc.other}@*)


        // Configure how much bandwidth the classes get
        htb () {                                            // Hierarchical Token Bucket packet scheduler(*@\label{lst:eth1.tcc.htb}@*)
                                                            // <http://luxik.cdi.cz/~devik/qos/htb/>

            class ( rate 200kbps, ceil 200kbps, burst 200000b, cburst 200000b ) {          // rate = how much is reserved, ceil = how much it can use max (*@\label{lst:eth1.tcc.eth1-speed}@*)
                // Allow $interactive to use up to 100kbps, but make sure there's always 50kbps available
                $interactive_out    = class ( rate 50kbps, ceil 100kbps, burst 50000b, cburst 100000b ) { sfq; } ; // Stochastic Fairness Queuing,(*@\label{lst:eth1.tcc.interactive-out-rate}@*)
                                                            // inhibits dominations of one connection inside the class 
                $interactive_in     = class ( rate 50kbps, ceil 100kbps, burst 50000b, cburst 100000b ) { sfq; } ; (*@\label{lst:eth1.tcc.interactive-in-rate}@*)
                $other     = class ( rate 100kbps, ceil 140kbps, burst 100000b, cburst 140000b ) { sfq; } ; (*@\label{lst:eth1.tcc.other-rate}@*)
            }
        }
    }
}
