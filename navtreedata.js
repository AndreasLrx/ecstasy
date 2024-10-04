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
    [ "ECSTASY (Entity Component System Toward Architecture Saving Years)", "index.html", null ],
    [ "Building", "md_doc_2_building.html", null ],
    [ "Glossary", "md_doc_2_glossary.html", [
      [ "Entity", "md_doc_2_glossary.html#autotoc_md15", null ],
      [ "Component", "md_doc_2_glossary.html#autotoc_md16", null ],
      [ "Storage", "md_doc_2_glossary.html#autotoc_md17", null ],
      [ "System", "md_doc_2_glossary.html#autotoc_md18", null ],
      [ "Resource", "md_doc_2_glossary.html#autotoc_md19", null ],
      [ "Registry", "md_doc_2_glossary.html#autotoc_md20", null ],
      [ "Query", "md_doc_2_glossary.html#autotoc_md21", [
        [ "Queryable", "md_doc_2_glossary.html#autotoc_md22", null ],
        [ "Batch Query", "md_doc_2_glossary.html#autotoc_md23", null ],
        [ "Modifier", "md_doc_2_glossary.html#autotoc_md24", null ],
        [ "Condition", "md_doc_2_glossary.html#autotoc_md25", null ]
      ] ],
      [ "Thread", "md_doc_2_glossary.html#autotoc_md26", [
        [ "Lockable", "md_doc_2_glossary.html#autotoc_md27", null ]
      ] ],
      [ "Serialization", "md_doc_2_glossary.html#autotoc_md28", [
        [ "Serializer actions", "md_doc_2_glossary.html#autotoc_md29", [
          [ "Export", "md_doc_2_glossary.html#autotoc_md30", null ],
          [ "Import", "md_doc_2_glossary.html#autotoc_md31", null ]
        ] ],
        [ "Variable actions", "md_doc_2_glossary.html#autotoc_md32", [
          [ "Serialize: Save", "md_doc_2_glossary.html#autotoc_md33", null ],
          [ "Deserialize: Update", "md_doc_2_glossary.html#autotoc_md34", null ],
          [ "Deserialize: Load", "md_doc_2_glossary.html#autotoc_md35", null ]
        ] ]
      ] ]
    ] ],
    [ "Tutorial", "md_doc_2_tutorial.html", [
      [ "Getting Started", "md_doc_2_tutorial.html#GettingStarted", null ],
      [ "Using entities", "md_doc_2_tutorial.html#autotoc_md37", [
        [ "Creating entities", "md_doc_2_tutorial.html#autotoc_md38", [
          [ "Using type resolution", "md_doc_2_tutorial.html#autotoc_md39", null ]
        ] ],
        [ "Manage entity component", "md_doc_2_tutorial.html#autotoc_md40", null ],
        [ "Delete entities", "md_doc_2_tutorial.html#autotoc_md41", null ]
      ] ],
      [ "Implement a custom storage", "md_doc_2_tutorial.html#CustomStorageTutorial", null ],
      [ "Making registry queries", "md_doc_2_tutorial.html#QueryTutorial", [
        [ "Select ... Where ...", "md_doc_2_tutorial.html#SelectWhereTutorial", null ],
        [ "Using modifiers", "md_doc_2_tutorial.html#ModifiersTutorial", null ],
        [ "Using conditions", "md_doc_2_tutorial.html#ConditionsTutorial", null ]
      ] ],
      [ "Using systems", "md_doc_2_tutorial.html#autotoc_md42", null ],
      [ "Ensuring Thread Safety", "md_doc_2_tutorial.html#EnsuringThreadSafety", null ],
      [ "Serializing your entities", "md_doc_2_tutorial.html#autotoc_md43", [
        [ "Using a serializer", "md_doc_2_tutorial.html#autotoc_md44", [
          [ "Common types", "md_doc_2_tutorial.html#autotoc_md45", null ],
          [ "Custom types", "md_doc_2_tutorial.html#autotoc_md46", null ],
          [ "Working with Entities (WIP)", "md_doc_2_tutorial.html#autotoc_md47", null ]
        ] ]
      ] ]
    ] ],
    [ "Technical Implementations", "md_doc_2_technical_implementation.html", [
      [ "Entities Management", "md_doc_2_technical_implementation.html#autotoc_md49", null ],
      [ "Components Management (MapStorage)", "md_doc_2_technical_implementation.html#autotoc_md50", null ],
      [ "Query", "md_doc_2_technical_implementation.html#autotoc_md51", null ]
    ] ],
    [ "Events integration", "md_doc_2integrations_2events.html", [
      [ "Description", "md_doc_2integrations_2events.html#autotoc_md60", [
        [ "How to get feedback", "md_doc_2integrations_2events.html#autotoc_md61", null ],
        [ "Event flow", "md_doc_2integrations_2events.html#autotoc_md62", null ]
      ] ],
      [ "Example", "md_doc_2integrations_2events.html#autotoc_md63", [
        [ "Creating listeners components", "md_doc_2integrations_2events.html#autotoc_md64", null ],
        [ "Sending an event", "md_doc_2integrations_2events.html#autotoc_md65", null ],
        [ "Accessing an input state", "md_doc_2integrations_2events.html#autotoc_md66", null ]
      ] ]
    ] ],
    [ "SFML integration", "md_doc_2integrations_2sfml.html", [
      [ "Description", "md_doc_2integrations_2sfml.html#autotoc_md68", null ],
      [ "Usage", "md_doc_2integrations_2sfml.html#autotoc_md69", null ]
    ] ],
    [ "User Actions integration", "md_doc_2integrations_2user__actions.html", [
      [ "Description", "md_doc_2integrations_2user__actions.html#autotoc_md71", null ],
      [ "Usage", "md_doc_2integrations_2user__actions.html#autotoc_md72", [
        [ "Register your actions and bindings", "md_doc_2integrations_2user__actions.html#autotoc_md73", null ],
        [ "Listen to your actions", "md_doc_2integrations_2user__actions.html#autotoc_md74", [
          [ "ActionListener", "md_doc_2integrations_2user__actions.html#autotoc_md75", null ],
          [ "ActionListeners", "md_doc_2integrations_2user__actions.html#autotoc_md76", null ],
          [ "ActionIdListener", "md_doc_2integrations_2user__actions.html#autotoc_md77", null ]
        ] ],
        [ "Using intermediate actions", "md_doc_2integrations_2user__actions.html#autotoc_md78", null ],
        [ "Saving and loading the bindings", "md_doc_2integrations_2user__actions.html#autotoc_md79", [
          [ "Saving", "md_doc_2integrations_2user__actions.html#autotoc_md80", null ],
          [ "Loading", "md_doc_2integrations_2user__actions.html#autotoc_md81", null ]
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
"_serializable_enum_8hpp.html#a61af11852be6ce892f96a1f6d63fd680",
"classecstasy_1_1_instances.html#a737c3f183c231185227ade1b3cdb2426",
"classecstasy_1_1_registry_1_1_select.html#a7e25b1f7e5dcc0d4ae5520ba23a0f9b3",
"classecstasy_1_1integration_1_1event_1_1_gamepad_combination_listener.html#a018088b728308635936dc1efe7194f14",
"classecstasy_1_1integration_1_1sfml_1_1_render_window.html#a3f029428baf9a897ddb9fd00ab371ab5",
"classecstasy_1_1query_1_1modifier_1_1_binary_modifier.html#a310815c38f25a3f17ff5199ca82d9ab2",
"classecstasy_1_1thread_1_1_lockable_view.html#a5cfdbd8821fad3d32e7018f28bcde418",
"classutil_1_1serialization_1_1_i_array_node.html#a8bc20ee4c4a8d62d0b106323976b4b37",
"classutil_1_1serialization_1_1_serializer.html#a778105300738c741d9e9fac79b33f0f6",
"cpp/algorithm/iota.html",
"cpp/experimental/forward_list/erase.html",
"cpp/io/c/ungetwc.html",
"cpp/numeric/math/round.html",
"cpp/types/is_constructible.html",
"functions_b.html",
"namespaceutil_1_1meta.html#a7788458e2dbea1a3cfcceec47e042cb8",
"structecstasy_1_1integration_1_1event_1_1_mouse_move_event.html#aadea56a8c89ff05313e9390108c683d2",
"structecstasy_1_1serialization_1_1traits_1_1has__update__impl__for__type.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';