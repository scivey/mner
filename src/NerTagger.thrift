namespace cpp2 scivey.mner.services

enum EntityType {
    PERSON = 0,
    LOCATION = 1,
    ORGANIZATION = 2,
    MISC = 3
}

struct NerItem {
    1: required EntityType entityType;
    2: required string entityName;
    3: required double confidence;
}

service NerTagger {
    void ping(),
    list<NerItem> tag(1: string text, 2: double confidence)
}
