<shader file="src/lighting/phong/phong.hlsl">

<techniques>
	<techique name="t1">
		<VS entryPoint="t1_VsMain">
			<Defines>
				<item DEFINE_NAME="DEFINE_VALUE"/>
			</Defines>
		
			<InputLayout>
				<item semanticName="position" semanticIndex="0" stream="0" format="float3"/>
				<item semanticName="texcoord" semanticIndex="0" stream="0" format="float2"/>
			</InputLayout>

			<InputBuffers>
				<item type="Matrix4x4"/>
			</InputBuffers>
		</VS>

		<PS entryPoint="t1_psMain">
			<Defines>
				<item name="DEFINE_NAME" val="DEFINE_VALUE"/>
			</Defines>
			
			<Rasterizer>
				<item name="FillMode" value="Solid"/>
			</Rasterizer>

			<DepthStencil>
				<item zWrite="true"/>
			</DepthStencil>

			<BlendDescriptions>
				<item RT="0" enabled="true" ColorSrcBlend="ONE" ColordstBlend="SRC_COLOR" ColorOP="ADD" AlphaSrcBlend="DEST_ALPHA" AlphaDstBlend="SRC1_ALPHA" AlphaOp="MAX" writeMask="0xFF"/>
			</BlendDescriptions>

			<Outputs>
				<item RT="0" format="RGBA"/>
			</Outputs>
		</PS>
	</techique>
</techniques>
</shader>