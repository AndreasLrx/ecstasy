/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "ECSTASY", "index.html", [
    [ "ECSTASY (Entity Component System Toward Architecture Saving Years)", "index.html", "index" ],
    [ "Building", "md_doc_2_building.html", [
      [ "Prerequisites", "md_doc_2_building.html#prerequisites", null ],
      [ "Build Steps", "md_doc_2_building.html#build-steps", [
        [ "IDEs", "md_doc_2_building.html#ides", null ]
      ] ],
      [ "Using as a CMake package", "md_doc_2_building.html#using-as-a-cmake-package", null ],
      [ "CMake options", "md_doc_2_building.html#CMakeOptions", null ]
    ] ],
    [ "Glossary", "md_doc_2_glossary.html", [
      [ "Entity", "md_doc_2_glossary.html#entity", null ],
      [ "Component", "md_doc_2_glossary.html#component", null ],
      [ "Storage", "md_doc_2_glossary.html#storage", null ],
      [ "System", "md_doc_2_glossary.html#system", null ],
      [ "Resource", "md_doc_2_glossary.html#resource", null ],
      [ "Registry", "md_doc_2_glossary.html#registry", null ],
      [ "Query", "md_doc_2_glossary.html#query", [
        [ "Queryable", "md_doc_2_glossary.html#queryable", null ],
        [ "Batch Query", "md_doc_2_glossary.html#batch-query", null ],
        [ "Modifier", "md_doc_2_glossary.html#modifier", null ],
        [ "Condition", "md_doc_2_glossary.html#condition", null ]
      ] ],
      [ "Thread", "md_doc_2_glossary.html#thread", [
        [ "Lockable", "md_doc_2_glossary.html#lockable", null ]
      ] ],
      [ "Serialization", "md_doc_2_glossary.html#serialization", [
        [ "Serializer actions", "md_doc_2_glossary.html#serializer-actions", [
          [ "Export", "md_doc_2_glossary.html#export", null ],
          [ "Import", "md_doc_2_glossary.html#import", null ]
        ] ],
        [ "Variable actions", "md_doc_2_glossary.html#variable-actions", [
          [ "Serialize: Save", "md_doc_2_glossary.html#serialize-save", null ],
          [ "Deserialize: Update", "md_doc_2_glossary.html#deserialize-update", null ],
          [ "Deserialize: Load", "md_doc_2_glossary.html#deserialize-load", null ]
        ] ]
      ] ]
    ] ],
    [ "Tutorial", "md_doc_2_tutorial.html", [
      [ "Getting Started", "md_doc_2_tutorial.html#GettingStarted", null ],
      [ "Using entities", "md_doc_2_tutorial.html#using-entities", [
        [ "Creating entities", "md_doc_2_tutorial.html#creating-entities", [
          [ "Using type resolution", "md_doc_2_tutorial.html#using-type-resolution", null ]
        ] ],
        [ "Manage entity component", "md_doc_2_tutorial.html#manage-entity-component", null ],
        [ "Delete entities", "md_doc_2_tutorial.html#delete-entities", null ]
      ] ],
      [ "Implement a custom storage", "md_doc_2_tutorial.html#CustomStorageTutorial", null ],
      [ "Making registry queries", "md_doc_2_tutorial.html#QueryTutorial", [
        [ "Select ... Where ...", "md_doc_2_tutorial.html#SelectWhereTutorial", null ],
        [ "Using modifiers", "md_doc_2_tutorial.html#ModifiersTutorial", null ],
        [ "Using conditions", "md_doc_2_tutorial.html#ConditionsTutorial", null ]
      ] ],
      [ "Using systems", "md_doc_2_tutorial.html#using-systems", [
        [ "Pipeline", "md_doc_2_tutorial.html#pipeline", null ],
        [ "Timers", "md_doc_2_tutorial.html#timers", [
          [ "Interval", "md_doc_2_tutorial.html#interval", null ],
          [ "Rate", "md_doc_2_tutorial.html#rate", null ],
          [ "Tips when using timers on Systems and Phases", "md_doc_2_tutorial.html#tips-when-using-timers-on-systems-and-phases", null ]
        ] ]
      ] ],
      [ "Using resources", "md_doc_2_tutorial.html#using-resources", null ],
      [ "Ensuring Thread Safety", "md_doc_2_tutorial.html#EnsuringThreadSafety", null ],
      [ "Using cross platform RTTI", "md_doc_2_tutorial.html#using-cross-platform-rtti", [
        [ "Why use custom RTTI implementation", "md_doc_2_tutorial.html#why-use-custom-rtti-implementation", null ],
        [ "How to use it", "md_doc_2_tutorial.html#how-to-use-it", [
          [ "How is it stored", "md_doc_2_tutorial.html#how-is-it-stored", null ],
          [ "Registering types", "md_doc_2_tutorial.html#registering-types", null ],
          [ "Querying types", "md_doc_2_tutorial.html#querying-types", null ]
        ] ]
      ] ],
      [ "Serializing your entities", "md_doc_2_tutorial.html#serializing-your-entities", [
        [ "Using a serializer", "md_doc_2_tutorial.html#using-a-serializer", [
          [ "Common types", "md_doc_2_tutorial.html#common-types", null ],
          [ "Custom types", "md_doc_2_tutorial.html#custom-types", null ],
          [ "Working with Entities", "md_doc_2_tutorial.html#working-with-entities", null ]
        ] ]
      ] ]
    ] ],
    [ "Technical Implementations", "md_doc_2_technical_implementation.html", [
      [ "Entities Management", "md_doc_2_technical_implementation.html#entities-management", null ],
      [ "Components Management (MapStorage)", "md_doc_2_technical_implementation.html#components-management-mapstorage", null ],
      [ "Query", "md_doc_2_technical_implementation.html#query-1", [
        [ "Queryable and BitSet", "md_doc_2_technical_implementation.html#queryable-and-bitset", null ],
        [ "Registry basic query", "md_doc_2_technical_implementation.html#registry-basic-query", null ],
        [ "Select where", "md_doc_2_technical_implementation.html#select-where", null ],
        [ "Modifiers", "md_doc_2_technical_implementation.html#modifiers", null ],
        [ "Conditions", "md_doc_2_technical_implementation.html#conditions", null ],
        [ "AutoLock", "md_doc_2_technical_implementation.html#autolock", null ],
        [ "Resolution Order", "md_doc_2_technical_implementation.html#resolution-order", null ]
      ] ]
    ] ],
    [ "Events integration", "md_doc_2integrations_2events.html", [
      [ "Description", "md_doc_2integrations_2events.html#description", [
        [ "How to get feedback", "md_doc_2integrations_2events.html#how-to-get-feedback", null ],
        [ "Event flow", "md_doc_2integrations_2events.html#event-flow", null ]
      ] ],
      [ "Example", "md_doc_2integrations_2events.html#example", [
        [ "Creating listeners components", "md_doc_2integrations_2events.html#creating-listeners-components", null ],
        [ "Sending an event", "md_doc_2integrations_2events.html#sending-an-event", null ],
        [ "Accessing an input state", "md_doc_2integrations_2events.html#accessing-an-input-state", null ]
      ] ]
    ] ],
    [ "SFML integration", "md_doc_2integrations_2sfml.html", [
      [ "Description", "md_doc_2integrations_2sfml.html#description-1", null ],
      [ "Usage", "md_doc_2integrations_2sfml.html#usage", null ]
    ] ],
    [ "User Actions integration", "md_doc_2integrations_2user__actions.html", [
      [ "Description", "md_doc_2integrations_2user__actions.html#description-2", null ],
      [ "Usage", "md_doc_2integrations_2user__actions.html#usage-1", [
        [ "Register your actions and bindings", "md_doc_2integrations_2user__actions.html#register-your-actions-and-bindings", null ],
        [ "Listen to your actions", "md_doc_2integrations_2user__actions.html#listen-to-your-actions", [
          [ "ActionListener", "md_doc_2integrations_2user__actions.html#actionlistener", null ],
          [ "ActionListeners", "md_doc_2integrations_2user__actions.html#actionlisteners", null ],
          [ "ActionIdListener", "md_doc_2integrations_2user__actions.html#actionidlistener", null ]
        ] ],
        [ "Using intermediate actions", "md_doc_2integrations_2user__actions.html#using-intermediate-actions", null ],
        [ "Saving and loading the bindings", "md_doc_2integrations_2user__actions.html#saving-and-loading-the-bindings", [
          [ "Saving", "md_doc_2integrations_2user__actions.html#saving", null ],
          [ "Loading", "md_doc_2integrations_2user__actions.html#loading", null ]
        ] ]
      ] ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ]
      ] ]
    ] ],
    [ "Concepts", "concepts.html", "concepts" ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"01__basic__window_2main_8cpp.html",
"_serializable_enum_8hpp.html#acd9fbb4df4bc6927538efdd7ab95d7cd",
"classecstasy_1_1_instances.html#a115ed034917b6bcd3a0a0ab508bceb3e",
"classecstasy_1_1_registry.html#a8d598f5f47d1a481303a89579a0827f2",
"classecstasy_1_1_vector_storage.html#a5ade2210e123abff476d4148707b3094",
"classecstasy_1_1integration_1_1event_1_1_key_combination_listener.html#a7ecca4499ed73261c74918fe1abf96b0",
"classecstasy_1_1integration_1_1event_1_1_keyboard.html#af7a1ffe443594240aaf7d589a7558fe3ae902674982fc99aa343cdd94da7476c3",
"classecstasy_1_1query_1_1_query_implementation_3_01util_1_1meta_1_1_traits_3_01_first_00_01_othe85d7ffd57dbe8185bd42d6b1216253c8.html#a1026ee502720e32309b574deb8348e63",
"classecstasy_1_1rtti_1_1_type.html#aedaa6265714188d9ec24104fda9cf20f",
"classecstasy_1_1serialization_1_1_serializer.html#aa8ad6f4c995f7185cbbf3b3100550925",
"classutil_1_1serialization_1_1_a_node.html#a9cfea3cd840f25d07ad07723e1fc1de2",
"classutil_1_1serialization_1_1_polymorphic_iterator.html#a668f931c08d101a2b5943d925fe9d4c6",
"concepts.html",
"cpp/chrono/c/timespec.html",
"cpp/io/c/fgetwc.html",
"cpp/numeric/math/exp.html",
"cpp/string/wide/wcstof.html",
"ecstasy_2query_2modifiers_2include_8hpp_source.html",
"md_doc_2_technical_implementation.html",
"namespacetoml.html#a5a5b291fdf3ecb9ad8bef4d6e689c402",
"structecstasy_1_1integration_1_1event_1_1_gamepad_button_released_event.html",
"structecstasy_1_1queryable__type_3_01_r_01_4.html#a19a90bbae0148beba23b958230c1f885"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';