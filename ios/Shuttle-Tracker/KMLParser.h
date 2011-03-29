//
//  KMLParser.h
//  Shuttle-Tracker
//
//  Created by Brendon Justin on 1/29/11.
//  Copyright 2011 Brendon Justin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>

@class KMLStyle, KMLPlacemark;

@interface KMLParser : NSObject <NSXMLParserDelegate> {
	NSMutableArray *_styles;
	NSMutableArray *_placemarks;

	NSMutableArray *_routes;
	NSMutableArray *_stops;

	NSMutableArray *_vehicles;

	NSURL *vehiclesUrl;

	KMLStyle *currentStyle;
	KMLPlacemark *currentPlacemark;

	//  Hold all of the characters found by the NSXMLParser for a given element
	NSMutableString *accumulation;

	struct {
		BOOL inStyle;
		BOOL inPlacemark;
		BOOL inNetworkLink;
	} state;

	@private
		NSURL *_parseUrl;
	NSXMLParser *_parser;
}

//  Do not return styles or placemarks by themselves
//@property (nonatomic, readonly) NSArray *styles;
//@property (nonatomic, readonly) NSArray *placemarks;
@property (nonatomic, copy) NSArray *routes;
@property (nonatomic, copy) NSArray *stops;
@property (nonatomic, copy) NSArray *vehicles;
@property (nonatomic, readonly) NSURL *vehiclesUrl;

- (id)initWithContentsOfUrl:(NSURL *)url;
- (void)parse;

@end


//  Use to hold style objects, I have only seen these used for routes
@interface KMLStyle : NSObject {
	NSString *idTag;
	NSString *colorString;
	UIColor *color;
	int width;

	enum KMLStyles {
		lineStyle,
		nilStyle
	} styleType;

	enum KMLStyleParseState {
		colorState,
		widthState,
		nilStyleParseState
	} parseState;
}

@property (nonatomic, retain) NSString *idTag;
@property (nonatomic, retain) NSString *colorString;
@property (nonatomic, readonly) UIColor *color;
@property (nonatomic) int width;
@property (nonatomic) enum KMLStyles styleType;
@property (nonatomic) enum KMLStyleParseState parseState;

@end

//  Base class for placemarks in KML, which indicate the location of something on a map;
//  subclasses must implement coordinates storage depending on their type
@interface KMLPlacemark : NSObject
{
	NSString *name;
	NSString *idTag;
	NSString *description;
	NSString *styleUrl;

	KMLStyle *style;

	enum PlacemarkType {
		routeType,
		pointType,
		stopType,
		vehicleType,
		nilType
	} placemarkType;

	enum KMLPlacemarkParseState {
		nameState,
		descriptionState,
		styleUrlState,
		coordinatesState,
		nilPlacemarkParseState
	} parseState;
}

@property (nonatomic, retain) NSString *name;
@property (nonatomic, readonly) NSString *title;
@property (nonatomic, retain) NSString *idTag;
@property (nonatomic, retain) NSString *description;
@property (nonatomic, readonly) NSString *subtitle;
@property (nonatomic, retain) NSString *styleUrl;
@property (nonatomic, retain) KMLStyle *style;
@property (nonatomic) enum PlacemarkType placemarkType;
@property (nonatomic) enum KMLPlacemarkParseState parseState;

@end


//  Routes consist of a list of coordinates, so use an array for coordinates storage
@interface KMLRoute : KMLPlacemark
{
	NSArray *lineString;
}

@property (nonatomic, retain) NSArray *lineString;


@end

#pragma mark -
#pragma mark Points

//  Base class for objects which have a single set of coordinates, so just use one set of coordinates
@interface KMLPoint : KMLPlacemark <MKAnnotation>
{
	CLLocationCoordinate2D coordinate;
	MKAnnotationView *annotationView;
}

@property (nonatomic) CLLocationCoordinate2D coordinate;
@property (nonatomic, retain) MKAnnotationView *annotationView;


- (id)initWithLocation:(CLLocationCoordinate2D)coord;

@end

@interface KMLStop : KMLPoint
{

}

@end

@interface KMLVehicle : KMLPoint
{

}

@end
