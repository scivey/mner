#mner

A c++ server exposing [MITIE](https://github.com/mit-nlp/MITIE)'s NER-tagging functionality as a [Thrift](https://github.com/facebook/fbthrift) service.

It uses fbthrift's new `cpp2` server to efficiently queue requests and hand them over to worker threads.

The workers themselves are long-lived and pre-load MITIE's NER model data on startup, significantly speeding up request processing.

Clients can be generated for any language with a thrift generator.
